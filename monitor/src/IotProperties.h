//
// Created by Ivan Kishchenko on 27.02.2022.
//

#pragma once

#include <string>
#include "properties/source/JsonPropertiesSource.h"
#include "properties/source/EnvPropertiesSource.h"

enum class IotType {
    ThingsBoard,
    Yandex,
};

inline void from_json(const nlohmann::json& j, IotType& type) {
    std::string str;
    j.get_to(str);
    if (str == "thingsboard") {
        type = IotType::ThingsBoard;
    } else {
        type = IotType::Yandex;
    }
}

struct IotProperties : Properties {
    std::string address;

    std::string clientId;
    std::string accessToken;

    std::string username;
    std::string password;

    std::string keyFile;

    // iot {
    IotType type;
    // } iot
};

inline void fromJson(JsonPropertiesSource& source, IotProperties& props) {
    if (auto it = source.getJson().find("iot-mqtt"); it != source.getJson().end()) {
        if (auto key = it->find("address"); key != it->end()) {
            key->get_to(props.address);
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

        if (auto key = it->find("type"); key != it->end()) {
            props.type = key.value();
        }

    }
}

inline void fromEnv(EnvPropertiesSource& source, IotProperties& props) {

}




