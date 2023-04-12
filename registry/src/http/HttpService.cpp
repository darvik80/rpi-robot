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
#include "resources/ResourceManager.h"
#include "rpc/RpcHealth.h"
#include "service/registry/RegistryRestController.h"
#include "service/device/DeviceRestController.h"
#include "service/device/DeviceTelemetryRestController.h"

void HttpService::addHandlers(Registry &registry, const HttpProperties &props) {
    auto jsonRpc = std::make_shared<JsonRpcHandler>();
    jsonRpc->addMethod(std::make_shared<RpcHealth>());

    registerHandler(http::verb::post, "/rpc", jsonRpc);
    auto registryRepository = std::make_shared<RegistryRestController>(registry.getService<Database>());
    registerHandler(http::verb::get, "/api/registries", registryRepository);
    registerHandler(http::verb::put, "/api/registries", registryRepository);
    registerHandler(http::verb::post, "/api/registries", registryRepository);
    registerHandler(http::verb::delete_, "/api/registries", registryRepository);
    registerHandler(http::verb::options, "/api/registries", registryRepository);

    auto deviceRepository = std::make_shared<DeviceRestController>(registry.getService<Database>());
    registerHandler(http::verb::get, "/api/devices", deviceRepository);
    registerHandler(http::verb::put, "/api/devices", deviceRepository);
    registerHandler(http::verb::post, "/api/devices", deviceRepository);
    registerHandler(http::verb::delete_, "/api/devices", deviceRepository);
    registerHandler(http::verb::options, "/api/devices", deviceRepository);

    auto telemetryRepository = std::make_shared<DeviceTelemetryRestController>(registry.getService<Database>());
    registerHandler(http::verb::get, "/api/devices-telemetry", telemetryRepository);
    registerHandler(http::verb::delete_, "/api/devices-telemetry", telemetryRepository);
    registerHandler(http::verb::options, "/api/devices-telemetry", telemetryRepository);

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

    registry.getService<EventBusService>().subscribe<ApplicationStartedEvent>(
            [props, worker, this](const auto &event) -> bool {
                worker->start();
                info("{} started: {}:{}", name(), props.host, props.port);
                return true;
            });

    registry.getService<EventBusService>().subscribe<ApplicationCloseEvent>(
            [props, worker, this](const auto &event) -> bool {
                worker->shutdown();
                info("{} stopped: {}:{}", name(), props.host, props.port);
                return true;
            });
}

void HttpService::process(const HttpRequest &req, HttpResponse &resp) {
    info(std::string("handle: ") + req.method_string().data() + ":" +
         std::string(req.target().data(), req.target().length()));
    HttpRequestHandlerManager::process(req, resp);
}

void HttpService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}