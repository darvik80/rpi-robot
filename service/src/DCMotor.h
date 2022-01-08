//
// Created by Ivan Kishchenko on 08.01.2022.
//

#pragma once

#include <BaseService.h>

class DCMotor : public BaseService {
public:
    DCMotor();

    const char *name() override;

    void postConstruct(Registry &registry) override;
};


