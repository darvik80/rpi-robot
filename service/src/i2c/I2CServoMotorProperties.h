//
// Created by Ivan Kishchenko on 20.02.2022.
//

#pragma once

#include <properties/source/EnvPropertiesSource.h>
#include <properties/source/JsonPropertiesSource.h>

struct I2CServoMotorProperties : Properties {
    std::string device="/dev/i2c-1";
    int deviceId{40};
    int channel{1};
};

inline void fromJson(JsonPropertiesSource& source, I2CServoMotorProperties& props) {
    if (auto it = source.getJson().find("i2c-servo"); it != source.getJson().end()) {
        if (auto key = it->find("device"); key != it->end()) {
            key->get_to(props.device);
        }
        if (auto key = it->find("device-id"); key != it->end()) {
            key->get_to(props.deviceId);
        }
    }
}

inline void fromEnv(EnvPropertiesSource& source, I2CServoMotorProperties& props) {

}


