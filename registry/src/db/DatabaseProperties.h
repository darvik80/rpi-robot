//
// Created by Ivan Kishchenko on 21/03/2023.
//

#pragma once

#include "properties/source/JsonPropertiesSource.h"
#include "properties/source/EnvPropertiesSource.h"

struct DatabaseProperties {
    std::string host = "127.0.0.1";
    int port = 5432;
    std::string username = "postgres";
    std::string password = "1234";
    std::string database = "postgres";
};

inline void fromJson(JsonPropertiesSource &source, DatabaseProperties &props) {
    if (auto it = source.getJson().find("database"); it != source.getJson().end()) {
        if (auto key = it->find("host"); key != it->end()) {
            key->get_to(props.host);
        }
        if (auto key = it->find("port"); key != it->end()) {
            key->get_to(props.port);
        }
        if (auto key = it->find("username"); key != it->end()) {
            key->get_to(props.username);
        }
        if (auto key = it->find("password"); key != it->end()) {
            key->get_to(props.password);
        }
        if (auto key = it->find("database"); key != it->end()) {
            key->get_to(props.database);
        }
    }
}

inline void fromEnv(EnvPropertiesSource &source, DatabaseProperties &props) {

}
