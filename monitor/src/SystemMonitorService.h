//
// Created by Ivan Kishchenko on 25.02.2022.
//

#pragma once

#include <BaseService.h>
#include <optional>

struct SystemInfoEvent {
    float cpuTemp{};
    std::optional<float> gpuTemp{};
};

#include <nlohmann/json.hpp>

void to_json(nlohmann::json &j, const SystemInfoEvent &e);

void from_json(const nlohmann::json &j, SystemInfoEvent &e);

class TemperatureSensor {
public:
    typedef std::unique_ptr<TemperatureSensor> Ptr;

    enum Type {
        CPU,
        GPU
    };

    virtual float getTemperature(Type type) = 0;

    virtual ~TemperatureSensor() = default;
};


class SystemMonitorService : public BaseServiceShared<SystemMonitorService> {
    TemperatureSensor::Ptr _sensor;
public:
    const char *name() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
};


