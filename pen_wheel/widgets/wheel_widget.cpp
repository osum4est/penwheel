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
    _selected_slice = -1;

    _wheel_size = pen_wheel::config()->wheel_size();
    _wheel_hole_size = pen_wheel::config()->wheel_hole_size();
    _slice_padding = pen_wheel::config()->slice_padding();
    _border_width = pen_wheel::config()->slice_border_width();

    _slices = (int) wheel->options().size();
    _slice_size = 360.0f / (float) _slices;
    _slice_offset = -_slice_size / 2;

    repaint();
}

void wheel_widget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    const pen_wheel_config *config = pen_wheel::config();
    const pen_wheel_palette *palette = config->find_palette(_wheel->palette());

    float cx = (float) geometry().width() / 2;
    float cy = (float) geometry().height() / 2;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.translate(cx, cy);

    QFont font(QString::fromStdString(config->font()));
    font.setPointSizeF(10);
    painter.setFont(font);

    for (int i = 0; i < _slices; i++) {
        const pen_wheel_option *option = &_wheel->options()[i];
        QColor color = palette && !option->color().set() ?
                       palette->at(i % palette->size()).color() : option->color().color();

        painter.setPen(QPen(color.darker(), _border_width));
        painter.setBrush(QBrush(_selected_slice == i ? color.lighter() : color));

        float start = 90 - _slice_offset;
        start -= (float) i * _slice_size;
        start -= _slice_padding / 2;
        float size = _slice_size - _slice_padding;

        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.arcTo(-_wheel_size / 2, -_wheel_size / 2, _wheel_size, _wheel_size, start, -size);
        path.closeSubpath();
        QPainterPath cut;
        cut.addEllipse(QPointF(0, 0), _wheel_hole_size / 2, _wheel_hole_size / 2);
        painter.drawPath(path.subtracted(cut));

        QString label = QString::fromStdString(option->name());
        float slice_width = _wheel_hole_size * qSin(size / _wheel_hole_size);
        float label_angle = start - (size / 2);
        float label_padding = slice_width / 5;
        float label_x = (_wheel_size / 2 + _wheel_hole_size / 2) / 2;
        float label_y = 0;

        font.setPixelSize(qRound(slice_width));
        painter.setFont(font);

        QRect label_size = painter.fontMetrics().boundingRect(label);
        float label_max_width = (_wheel_size / 2 - _wheel_hole_size / 2) - label_padding * 2;
        float factor = (label_max_width) / (float) label_size.width();
        if (factor > 1)
            factor = 1;

        painter.save();
        painter.rotate(-label_angle);
        painter.scale(factor, factor);
        painter.translate(qRound(label_x / factor), qRound(label_y / factor));
        if (label_angle + 90 <= 0) painter.rotate(180);
        painter.translate(qRound(-(float) label_size.width() / 2), qRound(-(float) label_size.height() / 2));

        painter.setPen(QPen(Qt::black, 10));
        painter.drawText(0, 0, label_size.width(), label_size.height(), Qt::AlignCenter, label);
        painter.restore();
    }
}

void wheel_widget::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);

    QPointF pos = event->position();
    QPointF center(geometry().width() / 2.0, geometry().height() / 2.0);

    QPointF rel = pos - center;
    rel.setY(-rel.y());

    int slice = -1;

    float min_dist = _wheel_hole_size / 2;
    if (rel.manhattanLength() > min_dist) {
        auto angle = (float) qRadiansToDegrees(qAtan2(rel.y(), rel.x()));
        float slice_angle = 360 - angle + 90 - _slice_offset;
        slice = int(float(int(slice_angle) % 360) / _slice_size);
    }

    if (slice == _selected_slice)
        return;

    _selected_slice = slice;
    repaint();

    if (_selected_slice >= 0) emit option_hover(&_wheel->options().at(_selected_slice));
    else emit option_hover(nullptr);
}

void wheel_widget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void wheel_widget::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);

    if (_selected_slice >= 0) emit option_selected(&_wheel->options().at(_selected_slice));
    else emit option_selected(nullptr);
    _selected_slice = -1;
}
