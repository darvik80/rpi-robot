//
// Created by Ivan Kishchenko on 08.01.2022.
//

#pragma once

#include "BaseService.h"
#include "I2CDriver.h"

class I2CServoMotor : public BaseService {
    std::unique_ptr<I2CDriver> _i2c;
private:
    void setPWMFreq(int freq);
    void setPWM(int channel, int off);
    void setServoPulse(int channel, int pulse);
public:
    I2CServoMotor();

    const char *name() override;

    void postConstruct(Registry &registry) override;
};
