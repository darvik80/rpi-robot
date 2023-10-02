//
// Created by Ivan Kishchenko on 02/10/2023.
//

#pragma once

#include <prometheus/exposer.h>
#include <prometheus/registry.h>

#include "BaseService.h"

struct PrometheusProperties {
    std::string host = "127.0.0.1";
    uint16_t port = 8081;
};

inline void fromJson(JsonPropertiesSource &source, PrometheusProperties &props) {
    if (auto it = source.getJson().find("prometheus"); it != source.getJson().end()) {
        if (auto key = it->find("host"); key != it->end()) {
            key->get_to(props.host);
        }
        if (auto key = it->find("port"); key != it->end()) {
            key->get_to(props.port);
        }
    }
}

inline void fromEnv(EnvPropertiesSource &source, PrometheusProperties &props) {

}

class PrometheusService : public BaseServiceShared<PrometheusService> {
    std::shared_ptr<prometheus::Exposer> _exposer;
    std::shared_ptr<prometheus::Registry> _registry;
public:
    PrometheusService();

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;

    const char *name() override {
        return "prometheus";
    }

    std::shared_ptr<prometheus::Registry> getRegistry() {
        return _registry;
    }
};
