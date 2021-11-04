#include "pen_wheel.h"

pen_wheel *pen_wheel::_instance = nullptr;

void pen_wheel::start() {
    _instance = this;

    std::filesystem::path config_file = "config.yaml";
    if (std::filesystem::is_regular_file(config_file))
        _config = std::make_unique<pen_wheel_config>("config.yaml");
    else
        _config = std::make_unique<pen_wheel_config>();

    _overlay = std::make_unique<pen_wheel_window>();
}

const pen_wheel_config *pen_wheel::config() {
    return _instance->_config.get();
}
