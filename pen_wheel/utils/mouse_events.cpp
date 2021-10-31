#include "mouse_events.h"
#include <iostream>
#include <qapplication.h>

bool mouse_events::run(mouse_event_handler *handler) {
    this->_handler = handler;
    return run();
}

void mouse_events::mouse_moved(const QPointF &pos) {
    if (_handler != nullptr)
        _handler->mouse_moved(pos);
}

void mouse_events::mouse_down(const Qt::MouseButton &button) {
    if (_handler != nullptr)
        _handler->mouse_down(button);
}

void mouse_events::mouse_up(const Qt::MouseButton &button) {
    if (_handler != nullptr)
        _handler->mouse_up(button);
}

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
            [](CGEventTapProxy, CGEventType type, CGEventRef event, void *user_info) {
                auto *self = (mouse_events *) user_info;
                switch (type) {
                    case kCGEventMouseMoved:
                        self->mouse_moved(QCursor::pos());
                        break;
                    case kCGEventLeftMouseDown:
                        self->mouse_down(Qt::LeftButton);
                        break;
                    case kCGEventLeftMouseUp:
                        self->mouse_up(Qt::LeftButton);
                        break;
                    case kCGEventLeftMouseDragged:
                        self->mouse_moved(QCursor::pos());
                        break;
                    case kCGEventRightMouseDown:
                        self->mouse_down(Qt::RightButton);
                        break;
                    case kCGEventRightMouseUp:
                        self->mouse_up(Qt::RightButton);
                        break;
                    case kCGEventRightMouseDragged:
                        self->mouse_moved(QCursor::pos());
                        break;
                    case kCGEventOtherMouseDown:
                        self->mouse_down(get_button_from_event(event));
                        break;
                    case kCGEventOtherMouseUp:
                        self->mouse_up(get_button_from_event(event));
                        break;
                    case kCGEventOtherMouseDragged:
                        self->mouse_moved(QCursor::pos());
                        break;
                    default:
                        break;
                }

                return event;
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