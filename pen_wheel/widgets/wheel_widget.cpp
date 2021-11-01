#include <QPainter>
#include <QMouseEvent>
#include "wheel_widget.h"
#include "ui_wheel_widget.h"

wheel_widget::wheel_widget(QWidget *parent) : QWidget(parent), _ui(new Ui::wheel_widget) {
    _ui->setupUi(this);
    setMouseTracking(true);
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

    float slice_size = 360.0f / (float) _slices;

    for (int i = 0; i < _slices; i++) {
        painter.setPen(QPen(_colors[i].darker(), border_width));

        if (_selected_slice == i)
            painter.setBrush(QBrush(_colors[i].lighter()));
        else
            painter.setBrush(QBrush(_colors[i]));

        float start = 90;
        start -= (float) i * slice_size;
        start -= angle_padding / 2;

        float size = slice_size - angle_padding / 2;

        painter.drawPie(x, y, width, height, int(start * 16), -int(size * 16));
    }

    int rim = 50;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white));
    painter.drawEllipse(x + rim, y + rim, width - rim - rim, height - rim - rim);
}

void wheel_widget::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);

    QPointF pos = event->position();
    QPointF center(geometry().width() / 2.0, geometry().height() / 2.0);

    QPointF rel = pos - center;
    rel.setY(-rel.y());

    int min_dist = geometry().width() / 6;
    if (rel.manhattanLength() < min_dist)
        return;

    auto angle = (float) qRadiansToDegrees(atan2(rel.y(), rel.x()));
    if (angle < 0)
        angle += 360;

    float slice_angle = 360 - angle + 90;
    if (slice_angle > 360)
        slice_angle -= 360;

    float slice_size = 360.0f / (float) _slices;
    int slice = (int) (slice_angle / slice_size);

    if (slice == _selected_slice)
        return;

    _selected_slice = slice;
    repaint();
}

void wheel_widget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void wheel_widget::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);

    qDebug() << "SELECTED: " << _selected_slice;
    _selected_slice = -1;
}
