//
// Created by Ivan Kishchenko on 07.01.2022.
//

#include "RobotApp.h"

#ifdef RASPBERRY_ARCH
#include "I2CServoDriver.h"
#include "DCMotor.h"
#include "joystick/JoystickService.h"
#include <pigpio.h>
#endif

const char *RobotApp::name() {
    return "robot";
}

void RobotApp::setup(Registry &registry) {
#ifdef RASPBERRY_ARCH
    if (gpioInitialise() == PI_INIT_FAILED) {
        error("can't init gpio...");
        exit(PI_INIT_FAILED);
        return;
    }

    registry.addService(std::make_shared<I2CServoDriver>());
    registry.addService(std::make_shared<DCMotor>());
    registry.addService(std::make_shared<xbox::JoystickService>());
#endif
}

void RobotApp::destroy(Registry &registry) {
}