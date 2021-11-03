#include "mouse_events.h"
#include <qapplication.h>

bool mouse_events::run(mouse_event_handler *handler) {
    this->_handler = handler;
    return run();
}

bool mouse_events::mouse_moved(const QPointF &pos) {
    return _handler != nullptr && _handler->mouse_moved(pos);
}

bool mouse_events::mouse_down(const Qt::MouseButton &button) {
    return _handler != nullptr && _handler->mouse_down(button);
}

bool mouse_events::mouse_up(const Qt::MouseButton &button) {
    return _handler != nullptr && _handler->mouse_up(button);
}

#ifdef WIN32

#include <Windows.h>
#include <windowsx.h>

HHOOK sHookMouse;
mouse_events *self = nullptr;

Qt::MouseButton get_button_from_lparam(LPARAM l) {
    switch (((MSLLHOOKSTRUCT *) l)->mouseData >> 16) {
        case XBUTTON1:
            return Qt::BackButton;
        case XBUTTON2:
            return Qt::ForwardButton;
    }
    return Qt::NoButton;
}

bool mouse_events::run() {
    self = this;
    sHookMouse = ::SetWindowsHookExA(
            WH_MOUSE_LL,
            [](int i, WPARAM w, LPARAM l) {
                if (i < 0)
                    return ::CallNextHookEx(sHookMouse, i, w, l);

                switch (w) {
                    case WM_MOUSEMOVE:
                        return self->mouse_moved(QCursor::pos()) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    case WM_LBUTTONDOWN:
                        return self->mouse_down(Qt::LeftButton) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    case WM_LBUTTONUP:
                        return self->mouse_up(Qt::LeftButton) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    case WM_RBUTTONDOWN:
                        return self->mouse_down(Qt::RightButton) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    case WM_RBUTTONUP:
                        return self->mouse_up(Qt::RightButton) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    case WM_MBUTTONDOWN:
                        return self->mouse_down(Qt::MiddleButton) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    case WM_MBUTTONUP:
                        return self->mouse_up(Qt::MiddleButton) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    case WM_XBUTTONDOWN:
                        return self->mouse_down(get_button_from_lparam(l)) ? TRUE : ::CallNextHookEx(sHookMouse, i, w,
                                                                                                     l);
                    case WM_XBUTTONUP:
                        return self->mouse_up(get_button_from_lparam(l)) ? TRUE : ::CallNextHookEx(sHookMouse, i, w, l);
                    default:
                        return ::CallNextHookEx(sHookMouse, i, w, l);
                }
            }, 0, 0);

    return sHookMouse != NULL;
}

#endif

#ifdef __APPLE__

#include <ApplicationServices/ApplicationServices.h>

Qt::MouseButton get_button_from_event(CGEventRef event) {
    switch (CGEventGetIntegerValueField(event, kCGMouseEventButtonNumber)) {
        case 2:
            return Qt::MiddleButton;
        case 3:
            return Qt::BackButton;
        case 4:
            return Qt::ForwardButton;
    }
    return Qt::NoButton;
}

bool mouse_events::run() {
    CGEventMask mask = CGEventMaskBit(kCGEventMouseMoved) |
                       CGEventMaskBit(kCGEventLeftMouseDown) |
                       CGEventMaskBit(kCGEventLeftMouseUp) |
                       CGEventMaskBit(kCGEventLeftMouseDragged) |
                       CGEventMaskBit(kCGEventRightMouseDown) |
                       CGEventMaskBit(kCGEventRightMouseUp) |
                       CGEventMaskBit(kCGEventRightMouseDragged) |
                       CGEventMaskBit(kCGEventOtherMouseDown) |
                       CGEventMaskBit(kCGEventOtherMouseUp) |
                       CGEventMaskBit(kCGEventOtherMouseDragged);

    CFMachPortRef event_tap = CGEventTapCreate(
            kCGSessionEventTap,
            kCGHeadInsertEventTap,
            kCGEventTapOptionDefault,
            mask,
            [](CGEventTapProxy, CGEventType type, CGEventRef event, void *user_info) -> CGEventRef {
                auto *self = (mouse_events *) user_info;
                switch (type) {
                    case kCGEventMouseMoved:
                    case kCGEventLeftMouseDragged:
                    case kCGEventRightMouseDragged:
                    case kCGEventOtherMouseDragged:
                        return self->mouse_moved(QCursor::pos()) ? nullptr : event;
                    case kCGEventLeftMouseDown:
                        return self->mouse_down(Qt::LeftButton) ? nullptr : event;
                    case kCGEventLeftMouseUp:
                        return self->mouse_up(Qt::LeftButton) ? nullptr : event;
                    case kCGEventRightMouseDown:
                        return self->mouse_down(Qt::RightButton) ? nullptr : event;
                    case kCGEventRightMouseUp:
                        return self->mouse_up(Qt::RightButton) ? nullptr : event;
                    case kCGEventOtherMouseDown:
                        return self->mouse_down(get_button_from_event(event)) ? nullptr : event;
                    case kCGEventOtherMouseUp:
                        return self->mouse_up(get_button_from_event(event)) ? nullptr : event;
                    default:
                        return event;
                }
            },
            this);

    if (event_tap == nullptr)
        return false;

    CFRunLoopAddSource(CFRunLoopGetCurrent(),
                       CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap, 0),
                       kCFRunLoopCommonModes);

    CGEventTapEnable(event_tap, true);
    return true;
}

#endif