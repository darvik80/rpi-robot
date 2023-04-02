//
// Created by Ivan Kishchenko on 26.04.2021.
//

#include "HttpService.h"
#include "HttpWorker.h"
#include "JsonRpcHandler.h"
#include "HttpFileHandler.h"
#include "HttpProperties.h"
#include "core-service/EventBusService.h"
#include "core-service/ApplicationEvent.h"
#include "../resources/ResourceManager.h"
#include "rpc/RpcHealth.h"

void HttpService::addHandlers(Registry &registry, const HttpProperties& props) {
    auto jsonRpc = std::make_shared<JsonRpcHandler>();
    jsonRpc->addMethod(std::make_shared<RpcHealth>());
    registry.getService<EventBusService>().send(JsonRpcRegisterEvent{*jsonRpc});

    registerHandler(http::verb::post, "/rpc", jsonRpc);

    std::string root = ResourceManager::instance().getResourcesDir();
    if (props.root.has_value()) {
        root.append("/");
        root.append(props.root.value());
    }

    registerHandler(http::verb::get, "/**", std::make_shared<HttpFileHandler>(root));
}

void HttpService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    const auto &props = registry.getProperties<HttpProperties>();

    addHandlers(registry, props);
    auto worker = std::make_shared<HttpWorker>(registry.getIoService(), props.host, props.port, shared_from_this());

    registry.getService<EventBusService>().subscribe<ApplicationStartedEvent>([&props, worker, this](const auto &event) -> bool {
        worker->start();
        info("started: " + props.host + ":" + std::to_string(props.port));
        return true;
    });

    registry.getService<EventBusService>().subscribe<ApplicationCloseEvent>([&props, worker, this](const auto &event) -> bool {
        worker->shutdown();
        info("stopped: " + props.host + ":" + std::to_string(props.port));
        return true;
    });
}

void HttpService::process(const HttpRequest& req, HttpResponse& resp) {
    info(std::string("handle: ") + req.method_string().data() + ":" + std::string(req.target().data(), req.target().length()));
    HttpRequestHandlerManager::process(req, resp);
}

void HttpService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}