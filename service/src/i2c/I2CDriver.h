//
// Created by Ivan Kishchenko on 19.02.2022.
//

#pragma once

#include <BaseService.h>

#include <memory>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

class I2CDriver {
    int _fd{};
private:
    int smBusAccess(char rw, uint8_t command, uint32_t size, i2c_smbus_data *data) const;

public:
    I2CDriver() = default;

    I2CDriver(std::string_view device, int subDevId);

    I2CDriver(I2CDriver &&other) noexcept;

    I2CDriver &operator=(I2CDriver &&other) noexcept;

    int read();

    int readReg8(int reg);

    int readReg16(int reg);

    int write(int data);

    int writeReg8(int reg, uint8_t data);

    int writeReg16(int reg, uint16_t data);

    virtual ~I2CDriver();
};


