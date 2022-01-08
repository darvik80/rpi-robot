//
// Created by Ivan Kishchenko on 08.01.2022.
//

#include "DCMotor.h"

LOG_COMPONENT_SETUP(motor, motor_logger)

DCMotor::DCMotor() : BaseService(motor_logger::get()) {}

const char *DCMotor::name() {
    return "dc-motor";
}

void DCMotor::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
}
