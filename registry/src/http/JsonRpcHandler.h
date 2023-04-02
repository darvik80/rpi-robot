//
// Created by Ivan Kishchenko on 26.04.2021.
//

#pragma once

#include "HttpRequestHandler.h"
#include "rpc/RpcRegistry.h"
#include "rpc/RpcMethod.h"

class JsonRpcHandler : public HttpRequestHandler, public RpcRegistry {
public:
    void handle(const HttpRequest &request, HttpResponse &response) override;

    void handle(const JsonRpcRequest &request, JsonRpcResponse &response);
};

class JsonRpcRegisterEvent {
    RpcRegistry &_jsonRpc;
public:
    explicit JsonRpcRegisterEvent(RpcRegistry &jsonRpc)
            : _jsonRpc(jsonRpc) {}

    [[nodiscard]] RpcRegistry &getJsonRpc() const {
        return _jsonRpc;
    }
};
