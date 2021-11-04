#include "pen_wheel_config.h"

const pen_wheel_color *pen_wheel_config::find_color(const std::string &name) const {
    return colors().contains(name) ? &colors().at(name) : nullptr;
}

const pen_wheel_palette *pen_wheel_config::find_palette(const std::string &name) const {
    return palettes().contains(name) ? &palettes().at(name) : nullptr;
}

const pen_wheel_wheel *pen_wheel_config::find_wheel(const std::string &name) const {
    return &*std::find_if(wheels().begin(), wheels().end(), [=](const pen_wheel_wheel &w) {
        return w.name() == name;
    });
}
