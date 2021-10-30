#include "wheel.h"
#include "ui_wheel.h"

wheel::wheel(QWidget *parent) : QDialog(parent), ui(new Ui::wheel) {
    ui->setupUi(this);
}

wheel::~wheel() {
    delete ui;
}

