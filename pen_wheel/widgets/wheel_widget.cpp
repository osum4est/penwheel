#include <QPainter>
#include "wheel_widget.h"
#include "ui_wheel_widget.h"

wheel_widget::wheel_widget(QWidget *parent) : QWidget(parent), _ui(new Ui::wheel_widget) {
    _ui->setupUi(this);
}

wheel_widget::~wheel_widget() {
    delete _ui;
}

void wheel_widget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    float border_width = 5;
    float angle_padding = 0;

    const int width = geometry().width() - border_width;
    const int height = geometry().height() - border_width;
    const int x = border_width / 2;
    const int y = border_width / 2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    int slices = 16;
    float slice_size = 360.0f / (float) slices;

    for (int i = 0; i < slices; i++) {
        painter.setPen(QPen(_colors[i].darker(), border_width));
        painter.setBrush(QBrush(_colors[i]));

        float start = 90;
        start -= (float) i * slice_size;
        start -= angle_padding / 2;

        float size = slice_size - angle_padding / 2;

        painter.drawPie(x, y, width, height, int(start * 16), -int(size * 16));

//        painter.drawtext
    }
}
