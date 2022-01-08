//
// Created by Ivan Kishchenko on 08.01.2022.
//

#include "I2CServoDriver.h"
#include <wiringPiI2C.h>

LOG_COMPONENT_SETUP(servo, servo_logger)

I2CServoDriver::I2CServoDriver() : BaseService(servo_logger::get()) {}


const char *I2CServoDriver::name() {
    return "i2c-servo";
}

// Registers/etc.
#define SUBADR1            0x02
#define SUBADR2            0x03
#define SUBADR3            0x04
#define MODE1              0x00
#define PRESCALE           0xFE
#define LED0_ON_L          0x06
#define LED0_ON_H          0x07
#define LED0_OFF_L         0x08
#define LED0_OFF_H         0x09
#define ALLLED_ON_L        0xFA
#define ALLLED_ON_H        0xFB
#define ALLLED_OFF_L       0xFC
#define ALLLED_OFF_H       0xFD

void I2CServoDriver::setPWMFreq(int freq) {
    double prescaleval = 25000000.0f;    // 25MHz
    prescaleval /= (4096*freq);               // 12-bit
    prescaleval -= 1.0;
    int preScale = (int) (prescaleval + 0.5);
    auto oldMode = wiringPiI2CReadReg8(_fd, MODE1);
    auto newMode = (oldMode & 0x7F) | 0x10;
    wiringPiI2CWriteReg8(_fd, MODE1, newMode);
    wiringPiI2CWriteReg8(_fd, PRESCALE, preScale);

    wiringPiI2CWriteReg8(_fd, MODE1, oldMode);
    usleep(5000);
    wiringPiI2CWriteReg8(_fd, MODE1, oldMode | 0x80);
}

void I2CServoDriver::setPWM(int channel, int on, int off) {
    wiringPiI2CWriteReg8(_fd, LED0_ON_L + 4 * channel, on & 0xFF);
    wiringPiI2CWriteReg8(_fd, LED0_ON_H + 4 * channel, on >> 8);

    wiringPiI2CWriteReg8(_fd, LED0_OFF_L + 4 * channel, off & 0xFF);
    wiringPiI2CWriteReg8(_fd, LED0_OFF_H + 4 * channel, off >> 8);
}

void I2CServoDriver::setServoPulse(int channel, int pulse) {
    setPWM(channel, 0, int(pulse * 4096 / 20000));
}

void I2CServoDriver::postConstruct(Registry &registry) {
    _fd = wiringPiI2CSetup(0x40);
    if (_fd == -1) {
        servo::log::error("i2c init failed");
        return;
    }
    wiringPiI2CWriteReg8(_fd, MODE1, 0x00);
    setPWMFreq(50);

    int i[] = {500, 2500, 10, 2500, 500, -10};
    for (int idx: i) {
        setServoPulse(0, idx);
        usleep(200000);
    }

    BaseService::postConstruct(registry);
}