#ifndef PEN_WHEEL_WHEEL_WIDGET_H
#define PEN_WHEEL_WHEEL_WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class wheel_widget; }
QT_END_NAMESPACE

class wheel_widget : public QWidget {
Q_OBJECT

public:
    explicit wheel_widget(QWidget *parent = nullptr);
    ~wheel_widget() override;

private:
    Ui::wheel_widget *ui;
};


#endif
