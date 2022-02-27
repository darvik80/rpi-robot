//
// Created by Ivan Kishchenko on 25.02.2022.
//

#pragma once

#include <BaseService.h>
#include "event/Event.h"

struct SystemInfoEvent : em::Event {
    double cpuTemp{};
    double gpuTemp{};

    SystemInfoEvent(em::EventSource::Ptr source, double cpuTemp, double gpuTemp)
            : Event(source), cpuTemp(cpuTemp), gpuTemp(gpuTemp) {}
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


