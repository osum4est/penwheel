#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QAbstractNativeEventFilter>

QT_BEGIN_NAMESPACE
namespace Ui { class main_window; }
QT_END_NAMESPACE

class main_window : public QMainWindow {
Q_OBJECT

    Ui::main_window *_ui;

public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window() override;


protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void open_wheel();


};

#endif
