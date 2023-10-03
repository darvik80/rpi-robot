//
// Created by Ivan Kishchenko on 02/10/2023.
//

#include "PrometheusService.h"
#include "core-service/EventBusService.h"
#include "iot/IotMessage.h"
#include <prometheus/gauge.h>
#include <prometheus/counter.h>

PrometheusService::PrometheusService() = default;

void PrometheusService::handleTelemetryJsonObject(prometheus::Registry &registry, std::string_view device, const nlohmann::json &json) {
    prometheus::Labels labels;
    for (auto & item: json.items()) {
        std::string name("telemetry_" + item.key());
        std::replace(name.begin(), name.end(), '-', '_');
        if (item.value().is_number()) {
            prometheus::BuildGauge()
                    .Name(name)
                    .Register(registry)
                    .Add({{"device_id", device.data()}})
                    .Set(item.value().get<double>());
        } else {
            prometheus::BuildCounter()
                    .Name(name)
                    .Labels({{"tag", item.value().get<std::string>()}})
                    .Register(registry)
                    .Add({{"device_id", device.data()}})
                    .Increment();
        }
    }
}

void PrometheusService::handleTelemetryJson(prometheus::Registry &registry, std::string_view device, const nlohmann::json &json) {
    if (json.is_object()) {
        handleTelemetryJsonObject(registry, device, json);
    } else {
        for (auto &item: json) {
            handleTelemetryJsonObject(registry, device, item);
        }
    }
}


void PrometheusService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto props = registry.getProperties<PrometheusProperties>();

    _exposer = std::make_shared<prometheus::Exposer>(fmt::format("{}:{}", props.host, props.port));
    _registry = std::make_shared<prometheus::Registry>();
    _exposer->RegisterCollectable(_registry);

    registry.getService<EventBusService>().subscribe<IotTelemetry>(
            [this, &registry](const IotTelemetry &event) -> bool {
                auto &prometheus = registry.getService<PrometheusService>();
                if (event.message.contains("status")) {
                    auto status = event.message["status"].get<std::string>();
                    auto &gauge = prometheus::BuildGauge()
                            .Name("devices")
                            .Register(*prometheus.getRegistry());
                    info("device: {}, status: {}", event.deviceId, status);
                    if (status == "online") {
                        gauge.Add({{"device_id", event.deviceId}}).Increment();
                    } else if (status == "offline") {
                        gauge.Add({{"device_id", event.deviceId}}).Decrement();
                    }
                } else {
                    handleTelemetryJson(*prometheus.getRegistry(), event.deviceId, event.message);
                }

                return true;
            }
    );
}

void PrometheusService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}

