//
// Created by Ivan Kishchenko on 2019-06-09.
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include <nlohmann/json.hpp>
#include "json/Spec.h"

class RpcMethod {
public:
    typedef std::shared_ptr<RpcMethod> Ptr;
    typedef std::vector<Ptr> PtrVec;
public:
    [[nodiscard]] virtual const char *name() const = 0;

    [[nodiscard]] virtual std::optional<nlohmann::json> handle(const std::optional<nlohmann::json> &params) const = 0;

    virtual ~RpcMethod() = default;
};

template<typename T, typename R>
class RpcFunction : public RpcMethod {
public:
    [[nodiscard]] std::optional<nlohmann::json> handle(const std::optional<nlohmann::json> &params) const override {
        T inc;
        if (params) {
            inc.unMarshal(params.value());
        }

        return marshal(exec(inc));
    };

    virtual R exec(const T &params) const = 0;
};

template<typename T>
class RpcConsumer : public RpcMethod {
public:
    [[nodiscard]] std::optional<nlohmann::json> handle(const std::optional<nlohmann::json> &params) const override {
        T inc;
        if (params) {
            from_json(params, inc);
        }

        exec(inc);

        return {};
    };

    virtual void exec(const T &params) const = 0;
};

template<typename R>
class RpcSupplier : public RpcMethod {
    [[nodiscard]] std::optional<nlohmann::json> handle(const std::optional<nlohmann::json> &params) const override {
        nlohmann::json res;

        to_json(res, exec());

        return res;
    };

    virtual R exec() const = 0;
};

class RpcExec : public RpcMethod {
    [[nodiscard]] std::optional<nlohmann::json> handle(const std::optional<nlohmann::json> &params) const override {
        nlohmann::json res;

        exec();

        return {};
    };

    virtual void exec() const = 0;
};
