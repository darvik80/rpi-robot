//
// Created by Ivan Kishchenko on 08.01.2022.
//

#pragma once

#include <BaseService.h>

class DCMotor : public BaseService {
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


