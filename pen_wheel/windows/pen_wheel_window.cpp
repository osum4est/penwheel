#include "pen_wheel_window.h"
#include "ui_pen_wheel_window.h"
#include <QMouseEvent>
#include <utils/processes.h>
#include <pen_wheel.h>
#include <utils/key_sender.h>

pen_wheel_window::pen_wheel_window(QWidget *parent) : QDialog(parent), _ui(new Ui::pen_wheel_window) {
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

    connect(_ui->wheel, &wheel_widget::option_hover, this, &pen_wheel_window::option_hover);
    connect(_ui->wheel, &wheel_widget::option_selected, this, &pen_wheel_window::option_selected);

    _mouse_events.run(this);
    set_state(hidden);
}

pen_wheel_window::~pen_wheel_window() {
    delete _ui;
}

void pen_wheel_window::open_wheel() {
    int size = qRound(pen_wheel::config()->wheel_size() + pen_wheel::config()->slice_border_width());
    setFixedWidth(size);
    setFixedHeight(size);
    _ui->wheel->setFixedWidth(size);
    _ui->wheel->setFixedHeight(size);

    std::string current_process = processes::get_active_process().name;
    const pen_wheel_wheel *wheel = nullptr;
    const auto &wheels = pen_wheel::config()->wheels();

    qDebug() << "Current app: " << QString::fromStdString(current_process);
    for (const auto &config_wheel: wheels) {
        for (const auto &process: config_wheel.processes()) {
            if (process == current_process) {
                wheel = &config_wheel;
                break;
            }
        }
        if (wheel != nullptr) break;
    }

    if (wheel == nullptr)
        wheel = pen_wheel::config()->find_wheel(pen_wheel::config()->single_press_wheel());

    if (wheel == nullptr) {
        set_state(hidden);
        return;
    }

    _ui->wheel->set_wheel(wheel);
    show();
    move_window(QCursor::pos());
}

void pen_wheel_window::option_hover(const pen_wheel_option *option) {
    _wheel_action_wheel = nullptr;

    if (option == nullptr) return;
    if (option->action().type() != pen_wheel_action::wheel) return;

    const pen_wheel_wheel *wheel = pen_wheel::config()->find_wheel(option->action().value());
    if (wheel == nullptr) return;
    _wheel_action_wheel = wheel;

    QThread *thread = QThread::create([this, wheel] {
        QThread::msleep(pen_wheel::config()->wheel_action_delay());
        if (_wheel_action_wheel != wheel || _state == floating) return;
        _wheel_action_wheel = nullptr;

        QMetaObject::invokeMethod(this, [this, wheel]() {
            _ui->wheel->set_wheel(wheel);
            move_window(QCursor::pos());
        }, Qt::QueuedConnection);
    });

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

void pen_wheel_window::option_selected(const pen_wheel_option *option) {
    if (option == nullptr) return;

    qDebug() << "Selected:" << QString::fromStdString(option->name());
    switch (option->action().type()) {
        case pen_wheel_action::none:
            break;
        case pen_wheel_action::press:
            _key_sender.send_key(pen_wheel_key_combination(option->action().value()));
            break;
        case pen_wheel_action::hold:
            _key_sender.hold_key(pen_wheel_key_combination(option->action().value()));
            break;
        case pen_wheel_action::wheel:
            // Handled when option is hovered over
            break;
    }
}

bool pen_wheel_window::mouse_moved(const QPointF &pos) {
    if (_state == floating)
        move_window(pos);

    // Since we are natively handing events (so that we don't take focus from the current app) we need to manually tell
    // qt about the event
    if (_state == active) {
        QMouseEvent evt(QEvent::MouseMove, pos - this->pos(), pos, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
        QApplication::sendEvent(_ui->wheel, &evt);
    }

    return false;
}

bool pen_wheel_window::mouse_down(const Qt::MouseButton &button) {
    if (_state == hidden && button == Qt::BackButton)
        set_state(floating);
    else if (_state == floating && button == Qt::LeftButton)
        set_state(active);

    if (_state != hidden) {
        QMouseEvent evt(QEvent::MouseButtonPress, QCursor::pos(), button, button, Qt::NoModifier);
        QApplication::sendEvent(_ui->wheel, &evt);
    }

    return _state != hidden;
}

bool pen_wheel_window::mouse_up(const Qt::MouseButton &button) {
    bool handled = _state != hidden;

    if (handled) {
        QMouseEvent evt(QEvent::MouseButtonRelease, QCursor::pos(), button, button, Qt::NoModifier);
        QApplication::sendEvent(_ui->wheel, &evt);
    }

    if (button == Qt::BackButton)
        set_state(hidden);
    else if (button == Qt::LeftButton && _state == active)
        set_state(floating);

    return handled;
}

void pen_wheel_window::move_window(const QPointF &pos) {
    int width = geometry().width();
    int height = geometry().height();
    setGeometry(pos.x() - width / 2, pos.y() - height / 2, width, height);
}

void pen_wheel_window::set_state(pen_wheel_window::wheel_state state) {
    _state = state;
    switch (state) {
        case hidden:
            hide();
            break;
        case floating:
            open_wheel();
            break;
        case active:
            show();
            move_window(QCursor::pos());
            break;
    }
}