#ifndef PEN_WHEEL_PEN_WHEEL_H
#define PEN_WHEEL_PEN_WHEEL_H

#include <config/pen_wheel_config.h>
#include <windows/pen_wheel_window.h>
#include <QFileSystemWatcher>

class pen_wheel : public QObject {
    static pen_wheel *_instance;

    std::filesystem::path _config_path;
    QFileSystemWatcher _config_watcher;
    std::unique_ptr<pen_wheel_config> _config;
    std::unique_ptr<pen_wheel_window> _overlay;

public:
    void start();

    static const pen_wheel_config *config();

private:
    void config_changed(const QString &path);
    void load_config();
};

#endif
