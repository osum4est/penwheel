#include "main_window.h"
#include "ui_main_window.h"
#include "wheel.h"
#include <QMouseEvent>
#include <utils/mouse_events.h>

main_window::main_window(QWidget *parent) : QMainWindow(parent), _ui(new Ui::main_window) {
    _ui->setupUi(this);

    connect(_ui->pushButton, &QPushButton::pressed, this, &main_window::open_wheel);

    auto wheel_window = new wheel();
    hide();

    pen_wheel_config config = pen_wheel_config("/Users/osum4est/Documents/Dev/PenWheel/res/default_config.yml");
    qDebug() << "NUMBER " << config.wheels().size();
}

main_window::~main_window() {
    delete _ui;
}

void main_window::open_wheel() {
}

void main_window::mousePressEvent(QMouseEvent *event) {
    qDebug() << event->button();

    QWidget::mousePressEvent(event);
}