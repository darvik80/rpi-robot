//
// Created by Ivan Kishchenko on 01.08.2021.
//

#pragma once

#include <event/Event.h>
#include <nlohmann/json.hpp>
#include "XboxJoystick.h"

namespace xbox {

    class JoystickEvent : public em::Event {
    };

    class Xbox380Event : public JoystickEvent, public XboxJoystick {
    };

    void to_json(nlohmann::json &j, const Xbox380Event &event);

    void from_json(const nlohmann::json &j, Xbox380Event &event);

}