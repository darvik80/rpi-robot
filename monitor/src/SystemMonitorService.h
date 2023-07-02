//
// Created by Ivan Kishchenko on 25.02.2022.
//

#pragma once

#include <BaseService.h>
#include <optional>
#include "TemperatureSensor.h"

struct SystemInfoEvent {
    std::optional<double> cpuTemp{};
    std::optional<double> gpuTemp{};
    std::optional<double> ambientTemp{};
};

#include <nlohmann/json.hpp>

void to_json(nlohmann::json &j, const SystemInfoEvent &e);

void from_json(const nlohmann::json &j, SystemInfoEvent &e);

class SystemMonitorService : public BaseServiceShared<SystemMonitorService> {
    TemperatureSensor _sensor{};
public:
    const char *name() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
};


