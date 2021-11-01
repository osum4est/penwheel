#ifndef PEN_WHEEL_WHEEL_H
#define PEN_WHEEL_WHEEL_H

#include <QDialog>
#include <QThread>
#include <utils/mouse_events.h>

QT_BEGIN_NAMESPACE
namespace Ui { class wheel; }
QT_END_NAMESPACE

class wheel : public QDialog, public mouse_event_handler{
Q_OBJECT

public:
    enum wheel_state {
        hidden,
        floating,
        active
    };

private:
    Ui::wheel *_ui;
    wheel_state _state;

public:
    explicit wheel(QWidget *parent = nullptr);
    ~wheel() override;

    bool mouse_moved(const QPointF &pos) override;
    bool mouse_down(const Qt::MouseButton &button) override;
    bool mouse_up(const Qt::MouseButton &button) override;

private:
    void set_state(wheel_state state);
    void move_window(const QPointF &pos);
};

#endif
