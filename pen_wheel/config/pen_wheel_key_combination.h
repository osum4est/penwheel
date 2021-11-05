#ifndef PEN_WHEEL_PEN_WHEEL_KEY_COMBINATION_H
#define PEN_WHEEL_PEN_WHEEL_KEY_COMBINATION_H

#include <QKeyCombination>
#include <string>
#include <yaml-cpp/yaml.h>

class pen_wheel_key_combination {
    friend YAML::convert<pen_wheel_key_combination>;

    std::string _combination_string;
    QKeyCombination _combination;

public:
    pen_wheel_key_combination() = default;
    explicit pen_wheel_key_combination(const std::string &combo);

    [[nodiscard]] Qt::KeyboardModifiers modifiers() const;
    [[nodiscard]] Qt::Key key() const;
};

namespace YAML {
    template<>
    struct convert<pen_wheel_key_combination> {
        static YAML::Node encode(const pen_wheel_key_combination &c) {
            return YAML::Node(c._combination_string);
        }

        static bool decode(const YAML::Node &j, pen_wheel_key_combination &c) {
            c = pen_wheel_key_combination(j.as<std::string>());
            return true;
        }
    };
}

#endif
