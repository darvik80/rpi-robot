//
// Created by Ivan Kishchenko on 07.01.2022.
//

#include "RobotApp.h"
#include <wiringPi.h>
#include "I2CServoDriver.h"
#include "DCMotor.h"
#include "joystick/JoystickService.h"

const char *RobotApp::name() {
    return "robot";
}

void RobotApp::setup(Registry &registry) {
    wiringPiSetup();

    registry.addService(std::make_shared<I2CServoDriver>());
    registry.addService(std::make_shared<DCMotor>());
    registry.addService(std::make_shared<xbox::JoystickService>());
}
