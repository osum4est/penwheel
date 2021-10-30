#include "wheel_widget.h"
#include "ui_wheel_widget.h"

wheel_widget::wheel_widget(QWidget *parent) : QWidget(parent), ui(new Ui::wheel_widget) {
    ui->setupUi(this);
}

wheel_widget::~wheel_widget() {
    delete ui;
}

