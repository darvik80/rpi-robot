//
// Created by Ivan Kishchenko on 08.01.2022.
//

#pragma once

#include <BaseService.h>
#include "gpiod/pwm.h"
#include <gpiod.hpp>

class DCMotor : public BaseService {
    gpiod::line _leftD1;
    gpiod::line _leftD2;
    gpiod::extension::soft_pwm _leftPwm;

    gpiod::line _rightD1;
    gpiod::line _rightD2;
    gpiod::extension::soft_pwm _rightPwm;
private:
    void stop();
    void forward(int speed);

    void right(int speed, bool dir);
    void left(int speed, bool dir);
public:
    DCMotor();

    const char *name() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;
};


