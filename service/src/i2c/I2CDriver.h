//
// Created by Ivan Kishchenko on 19.02.2022.
//

#pragma once

#include <BaseService.h>

#include <memory>

enum class DriverId {
    i2c,
};

class Driver {
public:
    virtual DriverId driverId() = 0;
};

class DriverManager : public BaseService {
public:
    const char *name() override {
        return "driver-manager";
    }

    template<typename T, typename... Args>
    T createDriver(Args &&... args) {
        T driver(std::forward<Args>(args)...);
        return driver;
    }
};

class I2CDriver : public Driver {
    int _fd{};
private:
    int smBusAccess(char rw, uint8_t command, int size, union i2c_smbus_data *data) const;

public:
    I2CDriver() = default;

    I2CDriver(std::string_view device, int subDevId);

    I2CDriver(I2CDriver &&other) noexcept;

    I2CDriver &operator=(I2CDriver &&other) noexcept;

    DriverId driverId() override {
        return DriverId::i2c;
    }

    int read();

    int readReg8(int reg);

    int readReg16(int reg);

    int write(int data);

    int writeReg8(int reg, uint8_t data);

    int writeReg16(int reg, uint16_t data);

    virtual ~I2CDriver();
};


