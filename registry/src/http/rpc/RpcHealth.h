//
// Created by Ivan Kishchenko on 27/03/2023.
//

#pragma once


#include "RpcMethod.h"

struct Health {
    std::string status;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Health, status)

class RpcHealth : public RpcSupplier<Health> {
public:
    [[nodiscard]] const char* name() const override {
        return "health";
    }

    [[nodiscard]] Health exec() const override {
        return Health{.status="UP"};
    }
};
