#ifndef PEN_WHEEL_PEN_WHEEL_H
#define PEN_WHEEL_PEN_WHEEL_H

#include <config/pen_wheel_config.h>
#include <windows/pen_wheel_window.h>

class pen_wheel {
    static pen_wheel *_instance;

    std::unique_ptr<pen_wheel_window> _overlay;
    std::unique_ptr<pen_wheel_config> _config;

public:
    void start();

    static const pen_wheel_config *config();
};

#endif
