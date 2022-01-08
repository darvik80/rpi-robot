//
// Created by Ivan Kishchenko on 08.01.2022.
//

#include "DCMotor.h"

LOG_COMPONENT_SETUP(motor, servo_logger)

DCMotor::DCMotor() : BaseService(servo_logger::get()) {}

const char *DCMotor::name() {
    return "dc-motor";
}

void DCMotor::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);
}
