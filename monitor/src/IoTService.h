//
// Created by Ivan Kishchenko on 26.02.2022.
//

#pragma once

#include "BaseService.h"
#include "network/boost/AsyncTcpClient.h"
#include "IoTProperties.h"

class IoTService : public BaseService  {
    using MqttClient = network::AsyncClient<network::SslSocket>;
    std::unique_ptr<MqttClient> _client;
    IoTProperties _props;
public:
    const char *name() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
};


