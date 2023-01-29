//
// Created by Ivan Kishchenko on 29.01.2023.
//


#pragma once


#include "BaseService.h"

class Srf05Sensor  : public BaseService {
    volatile int _shutdown = 0;
    std::thread _thread;
public:
    void postConstruct(Registry &registry) override;



    const char *name() override;

};
