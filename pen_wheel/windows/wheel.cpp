#include "wheel.h"
#include "ui_wheel.h"
#include <QMouseEvent>
#include <utils/mouse_events.h>

wheel::wheel(QWidget *parent) : QDialog(parent), _ui(new Ui::wheel) {
    _ui->setupUi(this);

    setWindowFlags(
            Qt::FramelessWindowHint |
            Qt::NoDropShadowWindowHint
    );

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    auto n = new mouse_events();
    n->run(this);

    _active = false;
    hide();
}

wheel::~wheel() {
    delete _ui;
}

void wheel::mouse_moved(const QPointF &pos) {
    if (!_active)
        return;

    move_window(pos);
}

void wheel::mouse_down(const Qt::MouseButton &button) {
    if (button == Qt::BackButton)
    {
        _active = true;
        show();
        raise();
        move_window(QCursor::pos());
    }
}

void wheel::mouse_up(const Qt::MouseButton &button) {
    if (button == Qt::BackButton)
    {
        _active = false;
        hide();
    }
}

void wheel::move_window(const QPointF pos) {
    int width = geometry().width();
    int height = geometry().height();
    setGeometry(pos.x() - width / 2, pos.y() - height / 2, width, height);
    grabMouse();
}
