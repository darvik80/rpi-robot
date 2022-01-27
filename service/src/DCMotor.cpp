//
// Created by Ivan Kishchenko on 08.01.2022.
//

#ifdef RASPBERRY_ARCH

#include "DCMotor.h"

#include "event/EventManagerService.h"
#include "joystick/JoystickEvent.h"
#include <pigpio.h>

// name         BCM
#define PWMA1   6
#define PWMA2   13
#define PWM1    26

#define PWMB1   20
#define PWMB2   21
#define PWM2    12

LOG_COMPONENT_SETUP(motor, motor_logger)

DCMotor::DCMotor()
        : BaseService(motor_logger::get()) {}

const char *DCMotor::name() {
    return "dc-motor";
}

void DCMotor::stop() {
    gpioWrite(PWMA1, PI_LOW);
    gpioWrite(PWMA2, PI_LOW);

    gpioWrite(PWMB1, PI_LOW);
    gpioWrite(PWMB2, PI_LOW);
}

void DCMotor::forward(int speed) {
    left(speed);
    right(speed);
}

void DCMotor::left(int speed) {
    debug("lt val: {}", speed);

    gpioWrite(PWMA1, PI_LOW);
    gpioWrite(PWMA2, PI_HIGH);

    if (int res = gpioPWM(PWM1, speed); res) {
        error("failed set PWM: {}", res);
    }
}

void DCMotor::right(int speed) {
    debug("rt val: {}", speed);

    gpioWrite(PWMB1, PI_LOW);
    gpioWrite(PWMB2, PI_HIGH);

    if (int res = gpioPWM(PWM2, speed); res) {
        error("failed set PWM: {}", res);
    }
}

void DCMotor::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    gpioSetMode(PWMA1, PI_OUTPUT);
    gpioSetMode(PWMA2, PI_OUTPUT);
    gpioSetMode(PWM1, PI_OUTPUT);

    gpioSetMode(PWMB1, PI_OUTPUT);
    gpioSetMode(PWMB2, PI_OUTPUT);
    gpioSetMode(PWM2, PI_OUTPUT);

    gpioSetPWMrange(PWM1, 1024);
    gpioSetPWMrange(PWM2, 1024);

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

//    softPwmStop(PWM1);
//    softPwmStop(PWM2);
}

#endif