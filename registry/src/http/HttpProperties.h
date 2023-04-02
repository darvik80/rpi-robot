//
// Created by Ivan Kishchenko on 24/03/2023.
//

#pragma once

#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include "properties/source/EnvPropertiesSource.h"
#include "properties/source/JsonPropertiesSource.h"

struct HttpProperties {
    std::string host = "0.0.0.0";
    uint16_t port = 8080;
    std::optional<std::string> root = "www";
};

inline void fromJson(JsonPropertiesSource &source, HttpProperties &props) {
    if (auto it = source.getJson().find("http"); it != source.getJson().end()) {
        if (auto key = it->find("host"); key != it->end()) {
            key->get_to(props.host);
        }
        if (auto key = it->find("port"); key != it->end()) {
            key->get_to(props.port);
        }
        if (auto key = it->find("root"); key != it->end()) {
            std::string root;
            key->get_to(root);
            props.root = root;
        }
    }
}

inline void fromEnv(EnvPropertiesSource &source, HttpProperties &props) {

}