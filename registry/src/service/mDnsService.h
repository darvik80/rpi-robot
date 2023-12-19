//
// Created by Ivan Kishchenko on 25/10/2023.
//

#pragma once

#include "BaseService.h"

struct mDnsProperties {

};


class mDnsService : public BaseServiceShared<mDnsService> {
public:
    mDnsService() = default;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;

    const char *name() override {
        return "m-dns";
    }

};
