//
// Created by Ivan Kishchenko on 07.01.2022.
//

#include "RobotApp.h"
#include "I2CServoDriver.h"
#include "DCMotor.h"
#include "joystick/JoystickService.h"
#include <pigpio.h>

const char *RobotApp::name() {
    return "robot";
}

void RobotApp::setup(Registry &registry) {
    if (setuid(getuid()) < 0) {
        printf("privilege change failed\n");
        exit(EXIT_FAILURE);
    }

    if (gpioInitialise() == PI_INIT_FAILED) {
        error("can't init gpio...");
        exit(PI_INIT_FAILED);
        return;
    }

    registry.addService(std::make_shared<I2CServoDriver>());
    registry.addService(std::make_shared<DCMotor>());
    registry.addService(std::make_shared<xbox::JoystickService>());
}

void RobotApp::destroy(Registry &registry) {
}