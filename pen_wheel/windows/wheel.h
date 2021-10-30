#ifndef PEN_WHEEL_WHEEL_H
#define PEN_WHEEL_WHEEL_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class wheel; }
QT_END_NAMESPACE

class wheel : public QDialog {
Q_OBJECT

    Ui::wheel *ui;

public:
    explicit wheel(QWidget *parent = nullptr);
    ~wheel() override;
};

#endif
