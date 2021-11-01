#include "wheel.h"
#include "ui_wheel.h"
#include <QMouseEvent>
#include <utils/mouse_events.h>

wheel::wheel(QWidget *parent) : QDialog(parent), _ui(new Ui::wheel) {
    _ui->setupUi(this);

    setWindowFlags(
            Qt::FramelessWindowHint |
            Qt::NoDropShadowWindowHint |
            Qt::WindowStaysOnTopHint |
            Qt::WindowDoesNotAcceptFocus |
            Qt::WindowTransparentForInput
    );

    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_ShowWithoutActivating, true);

    auto n = new mouse_events();
    n->run(this);
    set_state(hidden);
}

wheel::~wheel() {
    delete _ui;
}

bool wheel::mouse_moved(const QPointF &pos) {
    if (_state == floating)
        move_window(pos);

    // Since we are natively handing events (so that we don't take focus from the current app) we need to manually tell
    // qt about the event
    if (_state == active) {
        QMouseEvent evt(QEvent::MouseMove, pos - this->pos(), pos, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
        QApplication::sendEvent(_ui->wheel1, &evt);
    }

    return false;
}

bool wheel::mouse_down(const Qt::MouseButton &button) {
    if (_state == hidden && button == Qt::BackButton)
        set_state(floating);
    else if (_state == floating && button == Qt::LeftButton)
        set_state(active);

    // Since we are natively handing events (so that we don't take focus from the current app) we need to manually tell
    // qt about the event
    if (_state != hidden) {
        QMouseEvent evt(QEvent::MouseButtonPress, QCursor::pos(), button, button, Qt::NoModifier);
        QApplication::sendEvent(_ui->wheel1, &evt);
    }

    return _state != hidden;
}

bool wheel::mouse_up(const Qt::MouseButton &button) {
    bool handled = _state != hidden;

    if (button == Qt::BackButton)
        set_state(hidden);
    else if (button == Qt::LeftButton)
        set_state(hidden);

    // Since we are natively handing events (so that we don't take focus from the current app) we need to manually tell
    // qt about the event
    if (handled) {
        QMouseEvent evt(QEvent::MouseButtonRelease, QCursor::pos(), button, button, Qt::NoModifier);
        QApplication::sendEvent(_ui->wheel1, &evt);
    }

    return handled;
}

void wheel::move_window(const QPointF &pos) {
    int width = geometry().width();
    int height = geometry().height();
    setGeometry(pos.x() - width / 2, pos.y() - height / 2, width, height);
    grabMouse();
}

void wheel::set_state(wheel::wheel_state state) {
    _state = state;
    switch (state) {
        case hidden:
            hide();
            break;
        case floating:
        case active:
            show();
            move_window(QCursor::pos());
            break;
    }

}