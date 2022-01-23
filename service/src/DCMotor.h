//
// Created by Ivan Kishchenko on 08.01.2022.
//

#pragma once

#include <BaseService.h>

class DCMotor : public BaseService {
private:
    void stop();
    void forward(int speed);

    void right(int speed);
    void left(int speed);
public:
    DCMotor();

    const char *name() override;

    void postConstruct(Registry &registry) override;
};


