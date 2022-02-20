//
// Created by Ivan Kishchenko on 19.02.2022.
//

#include "I2CDriver.h"
#include <stdexcept>

#define I2C_SLAVE    0x0703
#define I2C_SMBUS    0x0720    /* SMBus-level access */

#define I2C_SMBUS_READ  1
#define I2C_SMBUS_WRITE 0

#define I2C_SMBUS_BYTE      1
#define I2C_SMBUS_BYTE_DATA 2
#define I2C_SMBUS_WORD_DATA 3


union i2c_smbus_data {
    uint8_t byte;
    uint16_t word;
};


struct i2c_smbus_ioctl_data {
    char read_write;
    uint8_t command;
    int size;
    i2c_smbus_data *data;
};

int I2CDriver::smBusAccess(char rw, uint8_t command, int size, union i2c_smbus_data *data) const {
    i2c_smbus_ioctl_data args{
            .read_write = rw,
            .command = command,
            .size = size,
            .data = data
    };

    return ioctl(_fd, I2C_SMBUS, &args);
}

I2CDriver::I2CDriver(std::string_view device, int subDevId) {
    if ((_fd = open(device.data(), O_RDWR)) < 0) {
        throw std::invalid_argument(fmt::format("failed open ic2 device: {}, {}", device, strerror(errno)));
    }

    if (ioctl(_fd, I2C_SLAVE, subDevId) < 0) {
        close(_fd);
        throw std::invalid_argument(fmt::format("unable to select I2C device: {}:{0:#x}, {}", device, subDevId, strerror(errno)));
    }
}

I2CDriver::I2CDriver(I2CDriver &&other) noexcept {
    std::swap(_fd, other._fd);
}

I2CDriver &I2CDriver::operator=(I2CDriver &&other) noexcept {
    if (this != &other) {
        if (_fd) {
            close(_fd);
        }
        _fd = other._fd;
        other._fd = 0;
    }

    return *this;
}

int I2CDriver::read() {
    i2c_smbus_data data{};

    if (smBusAccess(I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data)) {
        return -1;
    }

    return data.byte & 0xFF;
}

int I2CDriver::readReg8(int reg) {
    i2c_smbus_data data{};

    if (smBusAccess(I2C_SMBUS_READ, reg, I2C_SMBUS_BYTE_DATA, &data)) {
        return -1;
    }

    return data.byte & 0xFF;
}

int I2CDriver::readReg16(int reg) {
    i2c_smbus_data data{};

    if (smBusAccess(I2C_SMBUS_READ, reg, I2C_SMBUS_WORD_DATA, &data)) {
        return -1;
    }

    return data.byte & 0xFF;
}

int I2CDriver::write(int data) {
    return smBusAccess(I2C_SMBUS_WRITE, data, I2C_SMBUS_BYTE, nullptr);
}

int I2CDriver::writeReg8(int reg, uint8_t val) {
    i2c_smbus_data data{.byte = val};

    return smBusAccess(I2C_SMBUS_WRITE, reg, I2C_SMBUS_BYTE_DATA, &data);
}

int I2CDriver::writeReg16(int reg, uint16_t val) {
    i2c_smbus_data data{.word = val};

    return smBusAccess(I2C_SMBUS_WRITE, reg, I2C_SMBUS_WORD_DATA, &data);
}

I2CDriver::~I2CDriver() {
    close(_fd);
}
