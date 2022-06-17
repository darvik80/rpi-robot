//
// Created by Kishchenko Ivan on 17/06/2022.
//

#include "ThingsBoardIotService.h"

using namespace network;

void ThingsBoardIotService::onConnect(mqtt::MQTTAgent &agent) {
    std::string command = REQUEST_TOPIC + "/+";
    agent.callback(ATTRIBUTES_TOPIC, [this](auto &agent, std::string_view topic, std::string_view data) {
        onAttributes(agent, data);
    });
    agent.callback(CONFIG_TOPIC, [this](auto &agent, std::string_view topic, std::string_view data) {
        onConfig(agent, data);
    });
    agent.callback(command, [this, command](auto &agent, std::string_view topic, std::string_view data) {
        info("cmd: {}:{}", topic, data);
        std::string response = RESPONSE_TOPIC + topic.substr(REQUEST_TOPIC.size()).data();
        IotCommand cmd;
        from_json(nlohmann::json::parse(data), cmd);
        onCommand(agent, cmd);
        publish(response, 0, "success");
    });
    agent.subscribe(ATTRIBUTES_TOPIC, 1);
    agent.subscribe(CONFIG_TOPIC, 1);
    agent.subscribe(command, 1);

}

