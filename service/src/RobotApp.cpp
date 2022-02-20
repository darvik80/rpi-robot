//
// Created by Ivan Kishchenko on 07.01.2022.
//

#include "RobotApp.h"

#ifdef RASPBERRY_ARCH
#include "i2c/I2CDriver.h"
#include "i2c/I2CServoMotor.h"
#include "DCMotor.h"
#include "joystick/JoystickService.h"
#include <wiringPi.h>
#endif

const char *RobotApp::name() {
    return "robot";
}

void RobotApp::setup(Registry &registry) {
#ifdef RASPBERRY_ARCH
    if (-1 == wiringPiSetup()) {
        error("can't init wiringPiSetup");
        return;
    }

    registry.addService(std::make_shared<DriverManager>());
    registry.addService(std::make_shared<I2CServoMotor>());
    registry.addService(std::make_shared<DCMotor>());
    registry.addService(std::make_shared<JoystickService>());
#endif
}

void RobotApp::destroy(Registry &registry) {
}