#ifndef PEN_WHEEL_PEN_WHEEL_CONFIG_H
#define PEN_WHEEL_PEN_WHEEL_CONFIG_H

#include <cereal/cereal_yaml.h>
#include <config/pen_wheel_action.h>
#include <unordered_map>
#include "pen_wheel_color.h"

typedef std::map<std::string, pen_wheel_color> pen_wheel_colors;
typedef std::vector<pen_wheel_color> pen_wheel_palette;
typedef std::map<std::string, pen_wheel_palette> pen_wheel_palettes;

class pen_wheel_option {
    CEREAL_BEGIN(pen_wheel_option)
    CEREAL_PROP(name, std::string)
    CEREAL_PROP(action, pen_wheel_action)
    CEREAL_PROP(color, pen_wheel_color)
};

CEREAL_YAML_FINALIZE(pen_wheel_option)

class pen_wheel_wheel {
    CEREAL_BEGIN(pen_wheel_wheel)
    CEREAL_PROP_REQUIRED(name, std::string)
    CEREAL_PROP(palette, std::string)
    CEREAL_PROP(processes, std::vector<std::string>)
    CEREAL_PROP(options, std::vector<pen_wheel_option>)
};

CEREAL_YAML_FINALIZE(pen_wheel_wheel)

class pen_wheel_config {
    CEREAL_BEGIN(pen_wheel_config)
    CEREAL_PROP_DEFAULT(wheel_size, int, 250)
    CEREAL_PROP(single_press_shortcut, std::string)
    CEREAL_PROP(single_press_wheel, std::string)
    CEREAL_PROP(double_press_shortcut, std::string)
    CEREAL_PROP(double_press_wheel, std::string)
    CEREAL_PROP(colors, pen_wheel_colors)
    CEREAL_PROP(palettes, pen_wheel_palettes)
    CEREAL_PROP(wheels, std::vector<pen_wheel_wheel>)

public:
    const pen_wheel_color *find_color(const std::string &name) const;
    const pen_wheel_palette *find_palette(const std::string &name) const;
    const pen_wheel_wheel *find_wheel(const std::string &name) const;
};

CEREAL_YAML_FINALIZE(pen_wheel_config)

#endif
