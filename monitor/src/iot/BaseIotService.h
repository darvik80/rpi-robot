//
// Created by Kishchenko Ivan on 17/06/2022.
//

#ifndef RPI_ROBOT_BASEIOTSERVICE_H
#define RPI_ROBOT_BASEIOTSERVICE_H


#include "BaseService.h"
#include "../IotProperties.h"
#include "network/boost/AsyncTcpClient.h"
#include "network/mqtt/MQTTHandler.h"
#include "network/handler/NetworkLogger.h"
#include "network/mqtt/MQTTCodec.h"
#include "network/handler/IdleStateHandler.h"
#include "IotCommand.h"
#include "event/EventManagerService.h"
#include "IotMessage.h"

class BaseIotService : public BaseService {
protected:
    using MqttClient = network::AsyncClient<network::SslSocket>;
    std::unique_ptr<MqttClient> _client;
    network::mqtt::MQTTAgent::Ptr _agent;
    IotProperties _props;

    em::EventManager::Ptr _eventManager;
protected:
    virtual void onConnect(network::mqtt::MQTTAgent &agent) = 0;

    void onMessage(network::mqtt::MQTTAgent &agent, std::string_view topic, std::string_view data) {
        _eventManager->raiseEvent(IotMessage{topic, data});
    }

    void onAttributes(network::mqtt::MQTTAgent &agent, std::string_view data) {
        _eventManager->raiseEvent(IotMessage{"attributes", data});
    }

    void onConfig(network::mqtt::MQTTAgent &agent, std::string_view data) {
        _eventManager->raiseEvent(IotMessage{"config", data});
    }

    void onCommand(network::mqtt::MQTTAgent &agent, const IotCommand &cmd) {
        _eventManager->raiseEvent(cmd);
    }

public:
    const char *name() override {
        return "iot";
    }

    void postConstruct(Registry &registry) override {
        BaseService::postConstruct(registry);

        _eventManager = registry.getService<EventManagerService>().shared_from_this();
        _props = registry.getProperties<IotProperties>();

        auto &service = registry.getIoService();
        _agent = std::make_shared<network::mqtt::MQTTAgent>();
        _agent->connect([this](network::mqtt::MQTTAgent &agent) {
            onConnect(agent);
        });

        _client = std::make_unique<MqttClient>(
                service,
                [&service, this](const std::shared_ptr<network::AsyncChannel<network::SslSocket>> &channel) {
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
                            _agent
                    );
                },
                _props.keyFile
        );

        _client->connect(_props.address, 8883);
    }

    void preDestroy(Registry &registry) override {
        BaseService::preDestroy(registry);
        _client->shutdown();
        _client.reset();
    }

    virtual void publish(std::string_view topic, uint8_t qos, std::string_view data) {
        if (_agent) {
            info("pub: {}:{}", topic, data);
            _agent->publish(topic, qos, data);
        }
    }
    virtual void telemetry(uint8_t qos, std::string_view data) = 0;
};


#endif //RPI_ROBOT_BASEIOTSERVICE_H
