#include "main_window.h"
#include "ui_main_window.h"
#include "wheel.h"

main_window::main_window(QWidget *parent) : QMainWindow(parent), ui(new Ui::main_window) {
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::pressed, this, &main_window::open_wheel);
}

main_window::~main_window() {
    delete ui;
}

void main_window::open_wheel() {
    auto wheel_window = new wheel();
    wheel_window->show();
}
