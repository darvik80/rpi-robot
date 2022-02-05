//
// Created by Ivan Kishchenko on 08.01.2022.
//

#pragma once

#ifdef RASPBERRY_ARCH

#include <BaseService.h>

class I2CServoDriver : public BaseService {
    int _fd{0};
private:
    void setPWMFreq(int freq);
    void setPWM(int channel, int off);
    void setServoPulse(int channel, int pulse);
public:
    I2CServoDriver();

    const char *name() override;

    void postConstruct(Registry &registry) override;
};

#endif
