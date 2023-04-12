//
// Created by Ivan Kishchenko on 08.01.2022.
//

#include "DCMotor.h"
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
    _leftD1.set_value(0);
    _leftD2.set_value(0);
    _rightD1.set_value(0);
    _rightD2.set_value(0);
}

void DCMotor::forward(int speed) {
    left(speed, false);
    right(speed, false);
}

void DCMotor::left(int speed, bool dir) {
    info("lt val: {}:{}", speed, dir);

    if (dir) {
        _leftD1.set_value(0);
        _leftD2.set_value(1);
    } else {
        _leftD1.set_value(1);
        _leftD2.set_value(0);
    }
    _leftPwm.write(speed);
}

void DCMotor::right(int speed, bool dir) {
    info("rt val: {}:{}", speed, dir);

    if (dir) {
        _rightD1.set_value(1);
        _rightD2.set_value(0);
    } else {
        _rightD1.set_value(0);
        _rightD2.set_value(1);
    }
    _rightPwm.write(speed);
}

void DCMotor::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    gpiod::chip chip("gpiochip0");

    _leftD1 = chip.get_line(PWMA1);
    _leftD2 = chip.get_line(PWMA2);
    _leftPwm.setup(chip.get_line(PWM1), 0, 1024);
    _leftD1.request({"rpi-robot", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    _leftD2.request({"rpi-robot", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);

    _rightD1 = chip.get_line(PWMB1);
    _rightD2 = chip.get_line(PWMB2);
    _rightPwm.setup(chip.get_line(PWM2), 0, 1024);

    _rightD1.request({"rpi-robot", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);
    _rightD2.request({"rpi-robot", gpiod::line_request::DIRECTION_OUTPUT, 0}, 0);

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

    _leftPwm.shutdown();
    _rightPwm.shutdown();
}
