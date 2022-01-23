//
// Created by Ivan Kishchenko on 02.08.2021.
//

#pragma once

#include <string>
#include <string_view>

enum AxisId {
    axis_left,
    axis_mid,
    axis_right,
};

struct JoystickAxis {
    int x{0}, y{0};
};

class Joystick {
public:
    virtual const char* name() const = 0;
};
