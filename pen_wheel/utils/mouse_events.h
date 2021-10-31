#ifndef PEN_WHEEL_MOUSE_EVENTS_H
#define PEN_WHEEL_MOUSE_EVENTS_H

#include <QMouseEvent>

class mouse_event_handler {
public:
    virtual void mouse_moved(const QPointF &pos) = 0;
    virtual void mouse_down(const Qt::MouseButton &button) = 0;
    virtual void mouse_up(const Qt::MouseButton &button) = 0;
};

class mouse_events {
    mouse_event_handler *_handler;

public:
    bool run(mouse_event_handler *handler);

private:
    bool run();
    void mouse_moved(const QPointF &pos);
    void mouse_down(const Qt::MouseButton &button);
    void mouse_up(const Qt::MouseButton &button);
};


#endif
