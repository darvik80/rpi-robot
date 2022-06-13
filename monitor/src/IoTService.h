//
// Created by Ivan Kishchenko on 26.02.2022.
//

#pragma once

#include "BaseService.h"
#include "network/boost/AsyncTcpClient.h"
#include "IoTProperties.h"

#include "event/EventManagerService.h"
#include <network/handler/NetworkLogger.h>
#include <network/handler/IdleStateHandler.h>

#include <network/mqtt/MQTTHandler.h>
#include <network/mqtt/MQTTCodec.h>

#include "SystemMonitorService.h"


template<typename Socket>
class IoTService : public BaseService {
    using SocketType = Socket;
    using MqttClient = network::AsyncClient<Socket>;
    std::unique_ptr<MqttClient> _client;
    IoTProperties _props;
public:
    const char *name() override {
        return "iot";
    }

    void postConstruct(Registry &registry) override {
        BaseService::postConstruct(registry);

        auto &service = registry.getIoService();

        _props = registry.getProperties<IoTProperties>();

        auto agent = std::make_shared<network::mqtt::MQTTAgent>();
        agent->callback([this](auto &agent, std::string_view topic, std::string_view data) {
            info("{}:{}", topic, data);
        });
        agent->connect([](network::mqtt::MQTTAgent &agent) {
            agent.subscribe("v1/devices/me/rpc/request/+", 1);
        });
        _client = std::make_unique<MqttClient>(
                service,
                [agent, &service, this](const std::shared_ptr<network::AsyncChannel<SocketType>> &channel) {
                    link(
                            channel,
                            std::make_shared<network::handler::NetworkLogger>(),
                            std::make_shared<network::handler::IdleStateHandler>(service, boost::posix_time::seconds(5),
                                                                                 boost::posix_time::seconds(5)),
                            std::make_shared<network::mqtt::MQTTCodec>(
                                    network::mqtt::MQTTOptions{
                                            .clientId = _props.clientId,
                                            .accessToken = _props.accessToken
                                    }
                            ),
                            agent
                    );
                },
                _props.keyFile
        );

        if constexpr(std::is_base_of<network::SslSocket, Socket>::value) {
            _client->connect(_props.address, 8883);
        } else {
            _client->connect(_props.address, 1883);
        }

        registry.getService<EventManagerService>()
                .subscribe<SystemInfoEvent>(
                        [this, agent](const SystemInfoEvent &event) -> bool {
                            info("Temp: cpu: {}", event.cpuTemp);
                            agent->publish(_props.telemetryTopic, 1, to_string(nlohmann::json{event}));
                            return true;

                        }
                );
    }

    void preDestroy(Registry &registry) override {
        BaseService::preDestroy(registry);
        _client->shutdown();
        _client.reset();
    }
};


