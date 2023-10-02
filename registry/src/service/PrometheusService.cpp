//
// Created by Ivan Kishchenko on 02/10/2023.
//

#include "PrometheusService.h"

PrometheusService::PrometheusService() = default;

void PrometheusService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto props = registry.getProperties<PrometheusProperties>();

    _exposer = std::make_shared<prometheus::Exposer>(fmt::format("{}:{}", props.host, props.port));
    _registry = std::make_shared<prometheus::Registry>();
    _exposer->RegisterCollectable(_registry);
}

void PrometheusService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}

