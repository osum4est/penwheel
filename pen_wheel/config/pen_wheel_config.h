#ifndef PEN_WHEEL_PEN_WHEEL_CONFIG_H
#define PEN_WHEEL_PEN_WHEEL_CONFIG_H

#define CEREAL_CONFIG_PUBLIC

#include <cereal/cereal_yaml.h>

class pen_wheel_config_action {

    CEREAL_BEGIN(pen_wheel_config_action)
    CEREAL_PROP(name, std::string)
    CEREAL_PROP(action, std::string)
    CEREAL_PROP(color, int)
};

CEREAL_YAML_FINALIZE(pen_wheel_config_action)

class pen_wheel_config_wheel {
    CEREAL_BEGIN(pen_wheel_config_wheel)
    CEREAL_PROP(name, std::string)
    CEREAL_PROP_DEFAULT(main, bool, false)
    CEREAL_PROP_DEFAULT(processes, std::vector<std::string>, std::vector<std::string>())
    CEREAL_PROP(actions, std::vector<pen_wheel_config_action>)
};

CEREAL_YAML_FINALIZE(pen_wheel_config_wheel)

class pen_wheel_config {
    CEREAL_BEGIN(pen_wheel_config)
    CEREAL_PROP(wheels, std::vector<pen_wheel_config_wheel>)
};

#endif
