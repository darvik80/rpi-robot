//
// Created by Kishchenko Ivan on 17/06/2022.
//

#ifndef RPI_ROBOT_IOTCOMMAND_H
#define RPI_ROBOT_IOTCOMMAND_H

#include <system_error>
#include <nlohmann/json.hpp>

struct IotCommand : em::Event {
    std::string name;
    nlohmann::json params;
};

inline void from_json(const nlohmann::json& j, IotCommand& cmd) {
    j.at("method").get_to(cmd.name);
    j.at("params").get_to(cmd.params);
}


#endif //RPI_ROBOT_IOTCOMMAND_H
