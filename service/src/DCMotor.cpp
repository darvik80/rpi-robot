//
// Created by Ivan Kishchenko on 08.01.2022.
//

#include "DCMotor.h"
#include <wiringPi.h>
#include <softPwm.h>

#include "event/EventManagerService.h"
#include "joystick/JoystickEvent.h"

LOG_COMPONENT_SETUP(motor, motor_logger)

// name         wPO     BCM wPI
#define PWMA1   20  //  06  22
#define PWMA2   22  //  13  23
#define PWM1    21  //  12  26

#define PWMB1   26 //   20  28
#define PWMB2   27 //   21  29
#define PWM2    25 //   26  25

DCMotor::DCMotor()
        : BaseService(motor_logger::get()) {}

const char *DCMotor::name() {
    return "dc-motor";
}

void DCMotor::stop() {
    digitalWrite(PWMA1, LOW);
    digitalWrite(PWMA2, LOW);

    digitalWrite(PWMB1, LOW);
    digitalWrite(PWMB2, LOW);
}

void DCMotor::forward(int speed) {
    left(speed, false);
    right(speed, false);
}

void DCMotor::left(int speed, bool dir) {
    debug("lt val: {}", speed);

    if (dir) {
        digitalWrite(PWMA2, LOW);
        digitalWrite(PWMA1, HIGH);
    } else {
        digitalWrite(PWMA1, LOW);
        digitalWrite(PWMA2, HIGH);
    }
    softPwmWrite(PWM1, speed);
}

void DCMotor::right(int speed, bool dir) {
    debug("rt val: {}", speed);

    if (dir) {
        digitalWrite(PWMB2, LOW);
        digitalWrite(PWMB1, HIGH);
    } else {
        digitalWrite(PWMB1, LOW);
        digitalWrite(PWMB2, HIGH);
    }
    softPwmWrite(PWM2, speed);
}

void DCMotor::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    pinMode(PWMA1, OUTPUT);
    pinMode(PWMA2, OUTPUT);
    pinMode(PWM1, OUTPUT);

    pinMode(PWMB1, OUTPUT);
    pinMode(PWMB2, OUTPUT);
    pinMode(PWM2, OUTPUT);

    if (softPwmCreate(PWM1, 0, 1024)) {
        error("can't init PWM {}, errno: {}", PWM1, errno);
        return;
    }
    if (softPwmCreate(PWM2, 0, 1024)) {
        error("can't init PWM {}, errno: {}", PWM2, errno);
        return;
    }

    stop();

    registry.getService<EventManagerService>().subscribe<xbox::Xbox380Event>([this](const xbox::Xbox380Event &event) -> bool {
        debug("origin lt: {}:{}, rt: {}:{}", event.getLt(), event.isLb(), event.getRt(), event.isRb());
        left((event.getLt() + 32767) * 1024 / 65535, event.isLb());
        right((event.getRt() + 32767) * 1024 / 65535, event.isRb());

        return true;
    });

}

void DCMotor::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);

    softPwmStop(PWM1);
    softPwmStop(PWM2);
}
