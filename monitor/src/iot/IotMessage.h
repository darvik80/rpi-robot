//
// Created by Kishchenko Ivan on 17/06/2022.
//

#ifndef RPI_ROBOT_IOTMESSAGE_H
#define RPI_ROBOT_IOTMESSAGE_H

#include <string>
#include <event/Event.h>

struct IotMessage : em::Event {
    IotMessage(std::string_view topic, std::string_view message) : topic(topic), message(message) {}

    std::string topic;
    std::string message;
};


#endif //RPI_ROBOT_IOTMESSAGE_H
