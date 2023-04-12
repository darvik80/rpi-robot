//
// Created by Ivan Kishchenko on 18.02.2023.
//


#pragma once

#include "BaseService.h"
#include "I2CDriver.h"

class I2CLcd1602 : public BaseService {
    I2CDriver _i2c;
private:
    void sendCmd(uint8_t devAddr, uint8_t cmd);

    void sendData(uint8_t devAddr, char data);

public:
    const char *name() override {
        return "i2c-lcd1602";
    }

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
};
