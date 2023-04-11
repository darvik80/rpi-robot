//
// Created by Ivan Kishchenko on 07.01.2022.
//

#include "RobotApp.h"

#include "i2c/I2CServoMotor.h"
#include "DCMotor.h"
#include "joystick/JoystickService.h"
#include "Srf05Sensor.h"
#include "iot/IotPlatform.h"

const char *RobotApp::name() {
    return "robot";
}

void RobotApp::setup(Registry &registry) {
    registry.createService<Srf05Sensor>();
    registry.createService<I2CServoMotor>();
    registry.createService<DCMotor>();
    registry.createService<JoystickService>();
    registry.createService<IotDevice>();
}

void RobotApp::destroy(Registry &registry) {
}