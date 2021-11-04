#include <QApplication>
#include "pen_wheel.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    pen_wheel app;
    app.start();
    return QApplication::exec();
}
