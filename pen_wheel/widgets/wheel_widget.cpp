#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <pen_wheel.h>
#include "wheel_widget.h"
#include "ui_wheel_widget.h"

wheel_widget::wheel_widget(QWidget *parent) : QWidget(parent), _ui(new Ui::wheel_widget) {
    _ui->setupUi(this);
    setMouseTracking(true);
}

wheel_widget::~wheel_widget() {
    delete _ui;
}

void wheel_widget::set_wheel(const pen_wheel_wheel *wheel) {
    _wheel = wheel;
    _slices = (int) wheel->options().size();
    _slice_size = 360.0f / (float) _slices;
    _slice_offset = -_slice_size / 2;
    repaint();
}

void wheel_widget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    const pen_wheel_config *config = pen_wheel::config();

    float border_width = 3;
    float wheel_size = (float) config->wheel_size();

    float cx = geometry().width() / 2;
    float cy = geometry().height() / 2;

//    const int width = geometry().width() - border_width;
//    const int height = geometry().height() - border_width;
//    const int x = border_width / 2;
//    const int y = border_width / 2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.translate(cx, cy);

    const pen_wheel_palette *palette = config->find_palette(_wheel->palette());

    for (int i = 0; i < _slices; i++) {
        const pen_wheel_option *option = &_wheel->options()[i];
        QColor color =
                palette && !option->color().set() ? palette->at(i % palette->size()).color() : option->color().color();

        painter.setPen(QPen(color.darker(), border_width));
        painter.setBrush(QBrush(_selected_slice == i ? color.lighter() : color));

        float start = 90 - _slice_offset;
        start -= (float) i * _slice_size;
        start -= _slice_padding / 2;

        float size = _slice_size - _slice_padding;

        float sliceangle = start - (size / 2);
        float gap_distance = 50;

//        QPainterPath path;
//        path.setFillRule(Qt::WindingFill);
//        path.translate(geometry().width() / 2, geometry().height() / 2);
//        path.translate(
//                gap_distance * cos(qDegreesToRadians(sliceangle)),
//                -gap_distance * sin(qDegreesToRadians(sliceangle))
//                );
//        path.arcTo(x, y, width, height, start, -size);
//        path.closeSubpath();
//        painter.drawPath(path);
//
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.arcTo(-wheel_size / 2, -wheel_size / 2, wheel_size, wheel_size, start, -size);
        path.closeSubpath();
        QPainterPath cut;
        cut.addEllipse(QPointF(0, 0), gap_distance, gap_distance);
        painter.drawPath(path.subtracted(cut));

        int box_amount = _slices / 2 + 1;
        bool right = i < box_amount;
        int box_y = right ? i : _slices - i;
        float box_wheel_padding = 20;
        float box_height = 20;
        float box_width = 100;
        float height_with_boxes = wheel_size + box_wheel_padding + box_wheel_padding + box_height;

        float x;
        if (right)
            x = qMax(wheel_size / 2 * cos(qDegreesToRadians(start)),
                     wheel_size / 2 * cos(qDegreesToRadians(start - size)))
                + box_wheel_padding;
        else
            x = qMin(wheel_size / 2 * cos(qDegreesToRadians(start)),
                     wheel_size / 2 * cos(qDegreesToRadians(start - size)))
                - box_wheel_padding - box_width;

        if (box_y == 0 || (_slices % 2 == 0 && i == _slices / 2))
            x = -box_width / 2;

        float y = height_with_boxes / 2 + box_height / 2;
        y -= height_with_boxes / (box_amount - 1) * box_y;

        painter.drawRoundedRect(x, -y, box_width, box_height, 5, 5);

        QFont font;
        font.setPixelSize(16);
        painter.setPen(QPen(Qt::black, 10));
        painter.setFont(font);
        painter.drawText(
                x,
                -y,
                box_width,
                box_height,
                Qt::AlignCenter,
                QString::fromStdString(option->name())
        );
    }
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
    float slice_angle = 360 - angle + 90 - _slice_offset;

    int slice = int(float(int(slice_angle) % 360) / _slice_size);
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

    if (_selected_slice == -1)
        return;

    emit option_selected(&_wheel->options().at(_selected_slice));
    _selected_slice = -1;
}
