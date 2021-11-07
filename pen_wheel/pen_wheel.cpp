#include "pen_wheel.h"

pen_wheel *pen_wheel::_instance = nullptr;

void pen_wheel::start() {
    _instance = this;

    _config_path = "config.yaml";

    load_config();
    _config_watcher.addPath(QString::fromStdString(_config_path.string()));

    connect(&_config_watcher, &QFileSystemWatcher::fileChanged, this, &pen_wheel::config_changed);

    _overlay = std::make_unique<pen_wheel_window>();
}

const pen_wheel_config *pen_wheel::config() {
    return _instance->_config.get();
}

void pen_wheel::config_changed(const QString &path) {
    load_config();
}

void pen_wheel::load_config() {
    if (std::filesystem::is_regular_file(_config_path))
        _config = std::make_unique<pen_wheel_config>(_config_path);
    else
        _config = std::make_unique<pen_wheel_config>();
}
