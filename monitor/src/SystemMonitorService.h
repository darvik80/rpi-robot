//
// Created by Ivan Kishchenko on 25.02.2022.
//

#pragma once

#include <BaseService.h>
#include "event/Event.h"
#include <optional>

struct SystemInfoEvent : em::Event {
    float cpuTemp{};
    std::optional<float> gpuTemp{};
};

#include <nlohmann/json.hpp>

void to_json(nlohmann::json &j, const SystemInfoEvent &e);

void from_json(const nlohmann::json &j, SystemInfoEvent &e);

class SystemMonitorService : public BaseServiceShared<SystemMonitorService> {
public:
    const char *name() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
};


