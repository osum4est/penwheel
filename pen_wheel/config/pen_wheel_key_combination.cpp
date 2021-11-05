#include "pen_wheel_key_combination.h"
#include <QString>
#include <QKeySequence>

pen_wheel_key_combination::pen_wheel_key_combination(const std::string &combo) {
    _combination_string = combo;
    _combination = QKeySequence(QString::fromStdString(combo))[0];
}

Qt::KeyboardModifiers pen_wheel_key_combination::modifiers() const {
    return _combination.keyboardModifiers();
}

Qt::Key pen_wheel_key_combination::key() const {
    return _combination.key();
}
