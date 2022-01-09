//
// Created by Ivan Kishchenko on 02.08.2021.
//

#pragma once

struct JoystickAxis {
    int x{0}, y{0};
};

class Joystick {
    std::string _name;
public:
    explicit Joystick(const std::string &name)
            : _name(name) {}

    [[nodiscard]] const std::string &getName() const {
        return _name;
    }
};
