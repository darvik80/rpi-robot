//
// Created by Ivan Kishchenko on 26.04.2021.
//

#pragma once

#include "BaseService.h"
#include "HttpProperties.h"
#include "HttpRequestHandlerManager.h"

class HttpService : public BaseServiceShared<HttpService>, public HttpRequestHandlerManager {
public:
    const char *name() override {
        return "http";
    }

    int order() override {
        return INT32_MIN;
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;

    void process(const HttpRequest &req, HttpResponse &resp) override;

private:
    void addHandlers(Registry &registry, const HttpProperties &props);
};
