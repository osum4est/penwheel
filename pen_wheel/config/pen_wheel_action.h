#ifndef PEN_WHEEL_PEN_WHEEL_ACTION_H
#define PEN_WHEEL_PEN_WHEEL_ACTION_H

#include <string>
#include <yaml-cpp/yaml.h>

class pen_wheel_action {
    friend YAML::convert<pen_wheel_action>;

public:
    enum action_type {
        none,
        press,
        hold,
        wheel
    };

private:
    std::string _action;
    action_type _type = none;
    std::string _value;

public:
    pen_wheel_action() = default;
    explicit pen_wheel_action(const std::string &action);

    [[nodiscard]] action_type type() const;
    [[nodiscard]] std::string value() const;
};

namespace YAML {
    template<>
    struct convert<pen_wheel_action> {
        static YAML::Node encode(const pen_wheel_action &a) {
            return YAML::Node(a._action);
        }

        static bool decode(const YAML::Node &j, pen_wheel_action &a) {
            a = pen_wheel_action(j.as<std::string>());
            return true;
        }
    };
}

#endif
