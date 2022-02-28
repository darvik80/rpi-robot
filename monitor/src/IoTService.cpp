//
// Created by Ivan Kishchenko on 26.02.2022.
//

#include "IoTService.h"
#include "event/EventManagerService.h"
#include "SystemMonitorService.h"

#include <network/handler/NetworkLogger.h>
#include <network/handler/IdleStateHandler.h>

#include <network/mqtt/MQTTHandler.h>
#include <network/mqtt/MQTTCodec.h>

using namespace network;

const char *IoTService::name() {
    return "iot";
}

void IoTService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto &service = registry.getIoService();

    _props = registry.getProperties<IoTProperties>();

    auto agent = std::make_shared<mqtt::MQTTAgent>();
    agent->connect([](mqtt::MQTTAgent &agent) {
        agent.subscribe("v1/devices/me/rpc/request/+", 0);
    });
    _client = std::make_unique<MqttClient>(
            service,
            [agent, &service, this](const std::shared_ptr<AsyncChannel<network::SslSocket>> &channel) {
                link(
                        channel,
                        std::make_shared<handler::NetworkLogger>(),
                        std::make_shared<handler::IdleStateHandler>(service, boost::posix_time::seconds(5), boost::posix_time::seconds(5)),
                        std::make_shared<mqtt::MQTTCodec>(
                                mqtt::MQTTOptions{
                                    .clientId = _props.clientId,
                                    .accessToken = _props.accessToken
                                }
                        ),
                        agent
                );
            },
            _props.keyFile
    );

    _client->connect(_props.address, 8883);

    registry.getService<EventManagerService>().subscribe<SystemInfoEvent>([this, agent](const SystemInfoEvent &event) -> bool {
        info("Temp: cpu: {}", event.cpuTemp);

        agent->publish(_props.telemetryTopic, 0, to_string(nlohmann::json{event}));
        return true;
    });
}

void IoTService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
    _client->shutdown();
    _client.reset();
}
