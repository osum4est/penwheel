#ifndef PEN_WHEEL_PEN_WHEEL_COLOR_H
#define PEN_WHEEL_PEN_WHEEL_COLOR_H

#include <QColor>
#include <string>

class pen_wheel_color {
    friend YAML::convert<pen_wheel_color>;

    std::string _color_string;
    QColor _color = QColor(0xff00ff);
    bool _parsed = false;
    bool _set = false;

public:
    pen_wheel_color() = default;
    explicit pen_wheel_color(const std::string &color);

    [[nodiscard]] bool set() const;
    [[nodiscard]] QColor color() const;

private:
    void parse();
};

namespace YAML {
    template<>
    struct convert<pen_wheel_color> {
        static YAML::Node encode(const pen_wheel_color &c) {
            return YAML::Node(c._color_string);
        }

        static bool decode(const YAML::Node &j, pen_wheel_color &c) {
            c = pen_wheel_color(j.as<std::string>());
            return true;
        }
    };
}

#endif
