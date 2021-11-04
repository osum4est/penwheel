#include <regex>
#include <pen_wheel.h>
#include "pen_wheel_color.h"

pen_wheel_color::pen_wheel_color(const std::string &color) {
    _color_string = color;
    _set = true;
}

QColor pen_wheel_color::color() const {
    if (!_parsed)
        const_cast<pen_wheel_color *>(this)->parse();

    return _color;
}

bool pen_wheel_color::set() const {
    return _set;
}

void pen_wheel_color::parse() {
    std::string color = _color_string;
    color.erase(std::remove_if(color.begin(), color.end(), ::isspace), color.end());

    std::regex rgb_regex("rgb\\((.*),(.*),(.*)\\)", std::regex_constants::icase);
    std::regex rgba_regex("rgba\\((.*),(.*),(.*),(.*)\\)", std::regex_constants::icase);
    std::smatch match;

    try {
        if (pen_wheel::config()->colors().contains(color)) {
            _color = pen_wheel::config()->find_color(color)->color();
        } else if (std::regex_match(color, rgb_regex)) {
            std::regex_search(color, match, rgb_regex);
            _color = QColor(std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]));
        } else if (std::regex_match(color, rgba_regex)) {
            std::regex_search(color, match, rgba_regex);
            _color = QColor(std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3]), std::stoi(match[3]));
        } else {
            _color = QColor(QRgba64::fromArgb32(std::stoll(color, nullptr, 0)));
            if (_color.alpha() == 0)
                _color.setAlphaF(1);
        }
    } catch (const std::invalid_argument &e) { }

    _parsed = true;
}