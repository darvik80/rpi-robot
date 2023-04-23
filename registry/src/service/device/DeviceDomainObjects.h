//
// Created by Ivan Kishchenko on 11/04/2023.
//

#pragma once

#include <string>
#include <list>
#include <nlohmann/json.hpp>

struct DeviceConfigurationDo {
    long id{0};
    long deviceId{0};
    std::string createdAt;
    std::string updatedAt;
    int status{0};
    nlohmann::json json_data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceConfigurationDo, id, deviceId, createdAt, updatedAt, json_data)

struct DeviceDo {
    long id{0};
    long registryId{0};
    std::string createdAt;
    std::string updatedAt;
    std::string name;
    std::string uuid;
    int status{0};
    nlohmann::json json_data;
    std::list<DeviceConfigurationDo> configuration;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceDo, id, registryId, createdAt, updatedAt, name, uuid, status, json_data,
                                   configuration)
struct DeviceTelemetryDo {
    long id{0};
    long deviceId{0};
    DeviceDo refDevice;
    std::string createdAt;
    nlohmann::json json_data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceTelemetryDo, id, deviceId, refDevice, createdAt, json_data)

