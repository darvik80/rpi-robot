//
// Created by Ivan Kishchenko on 20.02.2022.
//

#pragma once

#include <properties/source/EnvPropertiesSource.h>
#include <properties/source/JsonPropertiesSource.h>

struct I2CServoMotorProperties : Properties {
    std::string device = "/dev/i2c-1";
    int deviceId{64};
    int channel{1};
};

inline void fromJson(JsonPropertiesSource &source, I2CServoMotorProperties &props) {
    if (auto it = source.getJson().find("i2c-servo"); it != source.getJson().end()) {
        for (auto itc = it->begin(); itc != it->end(); ++itc) {
            if (itc.key() == "device") {
                itc->get_to(props.device);
            } else if (itc.key() == "device-id") {
                itc->get_to(props.deviceId);
            } else if (itc.key() == "channel") {
                itc->get_to(props.channel);
            }
        }
    }
}

inline void fromEnv(EnvPropertiesSource &source, I2CServoMotorProperties &props) {

}


