//
// Created by Kishchenko Ivan on 17/06/2022.
//

#ifndef RPI_ROBOT_THINGSBOARDIOTSERVICE_H
#define RPI_ROBOT_THINGSBOARDIOTSERVICE_H

#include "BaseIotService.h"
#include "IotCommand.h"

class ThingsBoardIotService : public BaseIotService {
    const std::string TELEMETRY_TOPIC = "v1/devices/me/telemetry";
    const std::string ATTRIBUTES_TOPIC = "v1/devices/me/attributes";
    const std::string CONFIG_TOPIC = "v1/devices/me/config";
    const std::string REQUEST_TOPIC = "v1/devices/me/rpc/request";
    const std::string RESPONSE_TOPIC = "v1/devices/me/rpc/response";
protected:
    void onConnect(network::mqtt::MQTTAgent &agent) override;
public:
    const char *name() override {
        return "iot-thingsboard";
    }

    void telemetry(uint8_t qos, std::string_view data) override {
        publish(TELEMETRY_TOPIC, qos, data);
    }
};


#endif //RPI_ROBOT_THINGSBOARDIOTSERVICE_H
