//
// Created by Ivan Kishchenko on 07.01.2022.
//

#include "RobotApp.h"

#include "i2c/I2CDriver.h"
#include "i2c/I2CServoMotor.h"
#include "DCMotor.h"
#include "joystick/JoystickService.h"
#include "Srf05Sensor.h"
#include <wiringPi.h>

const char *RobotApp::name() {
    return "robot";
}

void RobotApp::setup(Registry &registry) {
    info("setup gpio");
    if (-1 == wiringPiSetupGpio()) {
        error("can't init wiringPiSetup");
        return;
    }
    info("setup services");
    registry.addService(std::make_shared<Srf05Sensor>());
    registry.addService(std::make_shared<I2CServoMotor>());
    registry.addService(std::make_shared<DCMotor>());
    registry.addService(std::make_shared<JoystickService>());
}

void RobotApp::destroy(Registry &registry) {
}