//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include "SystemMonitorService.h"
#include "core-service/SchedulerService.h"
#include "iot/IotMessage.h"
#include "iot/IotPlatform.h"
#include "nlohmann_optional.h"

void to_json(nlohmann::json &j, const SystemInfoEvent &e) {
    if (e.cpuTemp) {
        j["cpu-temp"] = e.gpuTemp.value();
    }
    if (e.gpuTemp) {
        j["gpu-temp"] = e.gpuTemp.value();
    }

    if (e.gpuTemp) {
        j["ambient-temp"] = e.ambientTemp.value();
    }
}

void from_json(const nlohmann::json &j, SystemInfoEvent &e) {
    if (j.contains("cpu-temp")) {
        std::optional<double> temp;
        j.at("cpu-temp").get_to(temp);
        if (temp) {
            e.cpuTemp = temp;
        }
    }
    if (j.contains("gpu-temp")) {
        float temp;
        j.at("gpu-temp").get_to(temp);
        e.gpuTemp = temp;
    }
}

const char *SystemMonitorService::name() {
    return "monitor";
}

void SystemMonitorService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto &iot = registry.getService<IotDevice>();

    registry.getService<SchedulerService>().scheduleAtFixedRate(
            [this, &iot]() -> void {
                SystemInfoEvent event{
                        .cpuTemp = _sensor.getTemperature(TemperatureSensor::Type::CPU),
                        .gpuTemp = _sensor.getTemperature(TemperatureSensor::Type::GPU),
                        .ambientTemp = _sensor.getTemperature(TemperatureSensor::Type::AMBIENT)
                };
                nlohmann::json json = event;
                logger::info("data: {}", json.dump());
                iot.telemetry(1, json.dump());
            },
            boost::posix_time::seconds{0},
            boost::posix_time::seconds{5}
    );
}

void SystemMonitorService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}

