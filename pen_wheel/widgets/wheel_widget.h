#ifndef PEN_WHEEL_WHEEL_WIDGET_H
#define PEN_WHEEL_WHEEL_WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class wheel_widget; }
QT_END_NAMESPACE

class wheel_widget : public QWidget {
Q_OBJECT

    Ui::wheel_widget *_ui;

    QColor _colors[16] = {
        QColor(0xab, 0x46, 0x42),
        QColor(0xdc, 0x96, 0x56),
        QColor(0xf7, 0xca, 0x88),
        QColor(0xa1, 0xb5, 0x6c),
        QColor(0x86, 0xc1, 0xb9),
        QColor(0x7c, 0xaf, 0xc2),
        QColor(0xba, 0x8b, 0xaf),
        QColor(0xa1, 0x69, 0x46),
        QColor(0xab, 0x46, 0x42),
        QColor(0xdc, 0x96, 0x56),
        QColor(0xf7, 0xca, 0x88),
        QColor(0xa1, 0xb5, 0x6c),
        QColor(0x86, 0xc1, 0xb9),
        QColor(0x7c, 0xaf, 0xc2),
        QColor(0xba, 0x8b, 0xaf),
        QColor(0xa1, 0x69, 0x46),
    };

    QString _labels[16] = {
            "Cut",
            "Copy",
            "Paste",
            "New Layer",
            "Delete Layer",
            "Ctrl",
            "Alt",
            "Shift",
            "Cut",
            "Copy",
            "Paste",
            "New Layer",
            "Delete Layer",
            "Ctrl",
            "Alt",
            "Shift"
    };

public:
    explicit wheel_widget(QWidget *parent = nullptr);
    ~wheel_widget() override;

protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif
