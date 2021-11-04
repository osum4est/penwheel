#ifndef PEN_WHEEL_PEN_WHEEL_WINDOW_H
#define PEN_WHEEL_PEN_WHEEL_WINDOW_H

#include <QDialog>
#include <QThread>
#include <utils/mouse_events.h>
#include <config/pen_wheel_config.h>

QT_BEGIN_NAMESPACE
namespace Ui { class pen_wheel_window; }
QT_END_NAMESPACE

class pen_wheel_window : public QDialog, public mouse_event_handler {
Q_OBJECT

public:
    enum wheel_state {
        hidden,
        floating,
        active
    };

private:
    Ui::pen_wheel_window *_ui;
    mouse_events _mouse_events;
    wheel_state _state;

public:
    explicit pen_wheel_window(QWidget *parent = nullptr);
    ~pen_wheel_window() override;

private:
    void open_wheel();
    void option_selected(const pen_wheel_option *option);

    bool mouse_moved(const QPointF &pos) override;
    bool mouse_down(const Qt::MouseButton &button) override;
    bool mouse_up(const Qt::MouseButton &button) override;

    void set_state(wheel_state state);
    void move_window(const QPointF &pos);
};

#endif