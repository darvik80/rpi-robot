//
// Created by Ivan Kishchenko on 08.01.2022.
//
#ifdef RASPBERRY_ARCH

#include "event/EventManagerService.h"
#include "joystick/JoystickEvent.h"

#include "I2CServoMotor.h"
#include "I2CServoMotorProperties.h"

LOG_COMPONENT_SETUP(servo, servo_logger)

I2CServoMotor::I2CServoMotor()
        : BaseService(servo_logger::get()) {}


const char *I2CServoMotor::name() {
    return "i2c-servo";
}

// Registers/etc.
#define MODE1              0x00
#define PRESCALE           0xFE

#define LED0_OFF_L         0x08
#define LED0_OFF_H         0x09

#include <cmath>

void I2CServoMotor::setPWMFreq(int freq) {
    int preScale = lroundf(25000000.0f / (4096 * freq));
    auto oldMode = _i2c.readReg8(MODE1);
    auto newMode = (oldMode & 0x7F) | 0x10;
    _i2c.writeReg8(MODE1, newMode);
    _i2c.writeReg8(PRESCALE, preScale);

    _i2c.writeReg8(MODE1, oldMode);
    usleep(5000);
    _i2c.writeReg8(MODE1, oldMode | 0x80);
}

void I2CServoMotor::setPWM(int channel, int off) {
    _i2c.writeReg8(LED0_OFF_L + 4 * channel, off & 0xFF);
    _i2c.writeReg8(LED0_OFF_H + 4 * channel, off >> 8);
}

void I2CServoMotor::setServoPulse(int channel, int pulse) {
    info("servo val: {}:{}", channel, pulse);
    setPWM(channel, pulse * 4096 / 20000);
}

void I2CServoMotor::postConstruct(Registry &registry) {
    auto props = registry.getProperties<I2CServoMotorProperties>();

    _i2c = registry.getService<DriverManager>().createDriver<I2CDriver>(props.device, props.deviceId);
    _i2c.writeReg8(MODE1, 0x00);

    setPWMFreq(50);

    registry.getService<EventManagerService>().subscribe<JoystickEvent>([this](const JoystickEvent &event) -> bool {
        setServoPulse(0, ((-event.axis[AxisId::axis_right].x + 32768) * 2000 / 65535) + 500);
        return true;
    });

    BaseService::postConstruct(registry);
}

#endif