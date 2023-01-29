//
// Created by Ivan Kishchenko on 08.01.2022.
//

#include "DCMotor.h"
#include <wiringPi.h>
#include <softPwm.h>

#include "core-service/EventBusService.h"
#include "joystick/JoystickEvent.h"

LOG_COMPONENT_SETUP(motor, motor_logger)

// name         wPI     BCM wPI wPO
#define PWM1    13  //  12  26  21
#define PWMA2   19  //  06  22  20
#define PWMA1   26  //  13  23  22

#define PWMB1   16 //   20  28  26
#define PWMB2   20 //   21  29  27
#define PWM2    21 //   26  25  25

DCMotor::DCMotor()
        : BaseService(motor_logger::get()) {
}

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
    info("lt val: {}:{}", speed, dir);

    if (dir) {
        digitalWrite(PWMA2, HIGH);
        digitalWrite(PWMA1, LOW);
    } else {
        digitalWrite(PWMA1, HIGH);
        digitalWrite(PWMA2, LOW);
    }
    softPwmWrite(PWM1, speed);
}

void DCMotor::right(int speed, bool dir) {
    info("rt val: {}:{}", speed, dir);

    if (dir) {
        digitalWrite(PWMB1, HIGH);
        digitalWrite(PWMB2, LOW);
    } else {
        digitalWrite(PWMB2, HIGH);
        digitalWrite(PWMB1, LOW);
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

    registry.getService<EventBusService>().subscribe<JoystickEvent>([this](const JoystickEvent &event) -> bool {
        info("origin lt: {}:{}, rt: {}:{}", event.lt, event.lb, event.rt, event.rb);

        if (event.type == JoystickType::gamepad) {
            auto ly = event.axis[AxisId::axis_left].y;
            auto ry = event.axis[AxisId::axis_right].y;
            left(std::abs(ly) * 1024 / 32767, ly <= 0);
            right(std::abs(ry) * 1024 / 32767, ry <= 0);
        } else {
            left((event.lt + 32767) * 1024 / 65535, event.lb);
            right((event.rt + 32767) * 1024 / 65535, event.rb);
        }

        return true;
    });

}

void DCMotor::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);

    softPwmStop(PWM1);
    softPwmStop(PWM2);
}
