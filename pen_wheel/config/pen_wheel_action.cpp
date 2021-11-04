#include "pen_wheel_action.h"

pen_wheel_action::pen_wheel_action(const std::string &action) : _action(action) {
    auto space_pos = action.find(' ');
    if (space_pos == std::string::npos)
        return;

    std::string type = action.substr(0, space_pos);
    _value = action.substr(space_pos + 1, action.size() - space_pos - 1);

    if (type == "press")
        _type = press;
    else if (type == "hold")
        _type = hold;
    else if (type == "pen_wheel_window")
        _type = wheel;

}

pen_wheel_action::action_type pen_wheel_action::type() const {
    return _type;
}

std::string pen_wheel_action::value() const {
    return _value;
}

