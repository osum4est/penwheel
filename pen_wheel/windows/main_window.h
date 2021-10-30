#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class main_window; }
QT_END_NAMESPACE

class main_window : public QMainWindow {
Q_OBJECT

    Ui::main_window *ui;

public:
    main_window(QWidget *parent = nullptr);
    ~main_window();

private:
    void open_wheel();
};

#endif
