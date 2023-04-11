//
// Created by Ivan Kishchenko on 11/04/2023.
//

#pragma once

#include <string>
#include <list>
#include <nlohmann/json.hpp>

struct DeviceConfigurationDo {
    long id;
    long deviceId;
    std::string createdAt;
    std::string updatedAt;
    int status;
    nlohmann::json json_data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceConfigurationDo, id, deviceId ,createdAt, updatedAt, json_data)

struct DeviceDo {
    long id;
    std::string createdAt;
    std::string updatedAt;
    std::string name;
    std::string uuid;
    int status;
    nlohmann::json json_data;
    std::list<DeviceConfigurationDo> configuration;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceDo, id, createdAt, updatedAt, name, uuid, status, json_data, configuration)

struct DeviceTelemetryDo {
    long id;
    long deviceId;
    DeviceDo refDevice;
    std::string createdAt;
    std::string updatedAt;
    nlohmann::json json_data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceTelemetryDo, id, deviceId ,createdAt, updatedAt, json_data)

