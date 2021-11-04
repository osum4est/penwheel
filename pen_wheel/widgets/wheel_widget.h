#ifndef PEN_WHEEL_WHEEL_WIDGET_H
#define PEN_WHEEL_WHEEL_WIDGET_H

#include <QWidget>
#include <config/pen_wheel_action.h>
#include <config/pen_wheel_config.h>

QT_BEGIN_NAMESPACE
namespace Ui { class wheel_widget; }
QT_END_NAMESPACE

class wheel_widget : public QWidget {
Q_OBJECT

    Ui::wheel_widget *_ui;

    int _slices = 0;
    float _slice_size = 0;
    float _slice_offset = 0;

    float _slice_length = 100;
    float _slice_padding = 5;

    int _selected_slice = -1;
    const pen_wheel_wheel *_wheel = nullptr;

public:
    explicit wheel_widget(QWidget *parent = nullptr);
    ~wheel_widget() override;

    void set_wheel(const pen_wheel_wheel *wheel);

signals:
    void option_selected(const pen_wheel_option *option);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif
