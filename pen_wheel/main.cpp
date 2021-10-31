#include <QApplication>
#include "windows/main_window.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    main_window w;
    w.clearFocus();
    w.hide();
    return QApplication::exec();
}
