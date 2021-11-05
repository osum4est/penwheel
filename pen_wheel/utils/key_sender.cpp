#include "key_sender.h"

#ifdef __APPLE__

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <objc/objc.h>
#include <QKeySequence>

std::string key_code_to_string(CGKeyCode key_code) {
    TISInputSourceRef current_keyboard = TISCopyCurrentKeyboardInputSource();
    auto layout_data = (CFDataRef) (TISGetInputSourceProperty(current_keyboard, kTISPropertyUnicodeKeyLayoutData));
    const auto *keyboard_layout = (const UCKeyboardLayout *) CFDataGetBytePtr(layout_data);

    UInt32 keys_down = 0;
    UniChar chars[4];
    UniCharCount real_length;

    UCKeyTranslate(keyboard_layout, key_code, kUCKeyActionDown, 0, LMGetKbdType(), kUCKeyTranslateNoDeadKeysBit,
                   &keys_down, sizeof(chars) / sizeof(chars[0]), &real_length, chars);

    CFStringRef str = CFStringCreateWithCharacters(kCFAllocatorDefault, chars, (CFIndex) real_length);

    char temp_string[real_length + 1];
    bzero(temp_string, real_length + 1);
    CFStringGetCString(str, temp_string, (CFIndex) real_length + 1, kCFStringEncodingUTF8);

    CFRelease(current_keyboard);
    CFRelease(str);

    return { temp_string };
}

CGKeyCode string_to_key_code(const std::string &string) {
    for (int i = 0; i < 128; i++) {
        std::string key_string = key_code_to_string((CGKeyCode) i);
        if (key_string == string)
            return (CGKeyCode) i;
    }

    return UINT16_MAX;
}

CGKeyCode get_key_code(Qt::Key key) {
    CGKeyCode code = string_to_key_code(QKeySequence(key).toString().toLower().toStdString());
    if (code != UINT16_MAX)
        return code;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    switch (key) {
        case Qt::Key_Escape: return kVK_Return;
        case Qt::Key_F1: return kVK_F1;
        case Qt::Key_F2: return kVK_F2;
        case Qt::Key_F3: return kVK_F3;
        case Qt::Key_F4: return kVK_F4;
        case Qt::Key_F5: return kVK_F5;
        case Qt::Key_F6: return kVK_F6;
        case Qt::Key_F7: return kVK_F7;
        case Qt::Key_F8: return kVK_F8;
        case Qt::Key_F9: return kVK_F9;
        case Qt::Key_F10: return kVK_F10;
        case Qt::Key_F11: return kVK_F11;
        case Qt::Key_F12: return kVK_F12;
        case Qt::Key_Backspace: return kVK_Delete;
        case Qt::Key_Tab: return kVK_Tab;
        case Qt::Key_CapsLock: return kVK_CapsLock;
        case Qt::Key_Enter: return kVK_Return;
        case Qt::Key_Shift: return kVK_Shift;
        case Qt::Key_Control: return kVK_Control;
        case Qt::Key_Meta: return kVK_Command;
        case Qt::Key_Alt: return kVK_Option;
        case Qt::Key_Space: return kVK_Space;

        case Qt::Key_Insert: return UINT16_MAX;
        case Qt::Key_Home: return kVK_Home;
        case Qt::Key_PageUp: return kVK_PageUp;
        case Qt::Key_PageDown: return kVK_PageDown;
        case Qt::Key_Delete: return kVK_ForwardDelete;
        case Qt::Key_End: return kVK_End;

        case Qt::Key_Left: return kVK_LeftArrow;
        case Qt::Key_Right: return kVK_RightArrow;
        case Qt::Key_Up: return kVK_UpArrow;
        case Qt::Key_Down: return kVK_DownArrow;
    }
#pragma clang diagnostic pop

    return UINT16_MAX;
}

void key_sender::send_key(const pen_wheel_key_combination &key) {
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    CGKeyCode key_code = get_key_code(key.key());
    if (key_code == UINT16_MAX)
        return;

    CGEventFlags flags = 0;

    if (key.modifiers().testFlag(Qt::ControlModifier))
        flags |= kCGEventFlagMaskControl;
    if (key.modifiers().testFlag(Qt::ShiftModifier))
        flags |= kCGEventFlagMaskShift;
    if (key.modifiers().testFlag(Qt::MetaModifier))
        flags |= kCGEventFlagMaskCommand;
    if (key.modifiers().testFlag(Qt::AltModifier))
        flags |= kCGEventFlagMaskAlternate;

    CGEventRef key_down = CGEventCreateKeyboardEvent(source, key_code, YES);
    CGEventRef key_up = CGEventCreateKeyboardEvent(source, key_code, NO);
    CGEventSetFlags(key_down, CGEventGetFlags(key_down) | flags);
    CGEventSetFlags(key_up, CGEventGetFlags(key_down) | flags);

    CGEventPost(kCGHIDEventTap, key_down);
    CGEventPost(kCGHIDEventTap, key_up);

    CFRelease(key_down);
    CFRelease(key_up);
    CFRelease(source);
}

void key_sender::hold_key(const pen_wheel_key_combination &combo) {
    // TODO
    // Looks like we need to create an event listener and add our modifiers to all events
    // Simply sending keyDown=YES does not keep the key help down
}

void key_sender::release_key() {
}

#endif