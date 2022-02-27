//
// Created by Ivan Kishchenko on 27.02.2022.
//

#pragma once

#include <string>
#include "properties/source/JsonPropertiesSource.h"
#include "properties/source/EnvPropertiesSource.h"

struct IoTProperties : Properties {
    std::string address;
    std::string telemetryTopic;

    std::string clientId;
    std::string accessToken;

    std::string username;
    std::string password;

    std::string keyFile;
};

inline void fromJson(JsonPropertiesSource& source, IoTProperties& props) {
    if (auto it = source.getJson().find("iot-mqtt"); it != source.getJson().end()) {
        if (auto key = it->find("address"); key != it->end()) {
            key->get_to(props.address);
        }
        if (auto key = it->find("telemetry-topic"); key != it->end()) {
            key->get_to(props.telemetryTopic);
        }
        if (auto key = it->find("access-token"); key != it->end()) {
            key->get_to(props.accessToken);
        }
        if (auto key = it->find("client-id"); key != it->end()) {
            key->get_to(props.clientId);
        }
        if (auto key = it->find("key-file"); key != it->end()) {
            key->get_to(props.keyFile);
        }
    }
}

inline void fromEnv(EnvPropertiesSource& source, IoTProperties& props) {

}




