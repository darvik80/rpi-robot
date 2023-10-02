//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>

#include "iot/IotPlatform.h"
#include "db/Database.h"
#include "http/HttpService.h"
#include "service/registry/RegistryRepository.h"
#include "service/device/DeviceRepository.h"
#include "service/device/DeviceTelemetryRepository.h"
#include "service/PrometheusService.h"

#include <prometheus/gauge.h>

class RegistryDatabase : public Database {
public:
    void createRepositories() override {
        createRepository<RegistryRepository>();
        createRepository<DeviceRepository>();
        createRepository<DeviceTelemetryRepository>();
    }
};

class RegistryApp : public Application {
public:
    const char *name() override {
        return "registry";
    }

protected:
    void setup(Registry &registry) override {
        registry.createService<IotRegistry>();
        registry.createService<RegistryDatabase>();
        registry.createService<HttpService>();
        registry.createService<PrometheusService>();

        registry.getService<EventBusService>().subscribe<IotTelemetry>(
                [this, &registry](const IotTelemetry &event) -> bool {
                    info("device: {}, telemetry: {}", event.deviceId, event.message.dump());
                    auto &db = registry.getService<RegistryDatabase>();

                    Filter filter;
                    filter.add("name", event.deviceId);
                    auto dev = db.getRepository<DeviceRepository>().findOne(filter);
                    if (dev) {
                        DeviceTelemetryDo telemetry{
                                .deviceId = dev->id,
                                .json_data = event.message
                        };
                        db.getRepository<DeviceTelemetryRepository>().insert(telemetry);
                    }

                    if (event.message.contains("status")) {
                        auto &prometheus = registry.getService<PrometheusService>();
                        std::string status = event.message["status"].get<std::string>();
                        auto &gauge = prometheus::BuildGauge()
                                .Name("devices")
                                .Register(*prometheus.getRegistry());
                        info("device: {}, status: {}", event.deviceId, status);
                        if (status == "online") {
                            gauge.Add({{"device_id", event.deviceId}}).Increment();
                        } else if (status == "offline") {
                            gauge.Add({{"device_id", event.deviceId}}).Decrement();
                        }
                    }

                    return true;
                });

    }

    void destroy(Registry &registry) override {

    }
};


int main(int argc, char *argv[]) {

    RegistryApp app;
    app.run(argc, argv);

    return 0;
}