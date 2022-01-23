//
// Created by Ivan Kishchenko on 08.01.2022.
//

#include "DCMotor.h"
#include <wiringPi.h>
#include <softPwm.h>

#include "event/EventManagerService.h"
#include "joystick/JoystickEvent.h"

LOG_COMPONENT_SETUP(motor, motor_logger)

// name         wPI     BCM
#define PWMA1   22 //   6
#define PWMA2   23 //   13
#define PWMB1   28 //   20
#define PWMB2   29 //   21
#define PWM1    26 //   12
#define PWM2    25 //   26

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
    left(speed);
    right(speed);
}

void DCMotor::left(int speed) {
    debug("lt val: {}", speed);

    digitalWrite(PWMA1, LOW);
    digitalWrite(PWMA2, HIGH);
    softPwmWrite(PWM1, speed);
}

void DCMotor::right(int speed) {
    debug("rt val: {}", speed);

    digitalWrite(PWMB1, LOW);
    digitalWrite(PWMB2, HIGH);
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
        debug("origin lt: {}, rt: {}", event.getLt(), event.getRt());
        left((event.getLt() + 32767) * 1024 / 65535);
        right((event.getRt() + 32767) * 1024 / 65535);

        return true;
    });

}

void DCMotor::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);

    softPwmStop(PWM1);
    softPwmStop(PWM2);
}
