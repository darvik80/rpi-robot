//
// Created by Ivan Kishchenko on 07.01.2022.
//

#pragma once

#include <Application.h>

class RobotApp : public Application {
public:
    const char *name() override;
protected:
    void setup(Registry &registry) override;
    void destroy(Registry &registry) override;
};


