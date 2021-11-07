#ifndef PEN_WHEEL_KEY_SENDER_H
#define PEN_WHEEL_KEY_SENDER_H

#include <string>
#include <vector>
#include <config/pen_wheel_key_combination.h>

class key_sender {
public:
    void send_key(const pen_wheel_key_combination &combo);
    void hold_key(const pen_wheel_key_combination &combo);
    void release_key(const pen_wheel_key_combination &combo);
};

#endif
