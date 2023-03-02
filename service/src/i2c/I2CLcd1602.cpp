//
// Created by Ivan Kishchenko on 18.02.2023.
//

#include "I2CLcd1602.h"

// commands
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// flags for function set
#define LCD_8BITMODE    0x10
#define LCD_4BITMODE    0x00
#define LCD_2LINE       0x08
#define LCD_1LINE       0x00
#define LCD_5x10DOTS    0x04
#define LCD_5x8DOTS     0x00

// flags for backlight control
#define LCD_BACKLIGHT   0x08
#define LCD_NOBACKLIGHT 0x00

void I2CLcd1602::sendCmd(uint8_t devAddr, uint8_t cmd) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C;  //en=1, rs=0
    data_t[1] = data_u | 0x08;  //en=0, rs=0
    data_t[2] = data_l | 0x0C;  //en=1, rs=0
    data_t[3] = data_l | 0x08;  //en=0, rs=0

//    _i2c.write()
    //ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, devAddr, data_t, 4, 1000));
}

void I2CLcd1602::sendData(uint8_t devAddr, char data) {
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;  //en=1, rs=0
    data_t[1] = data_u | 0x09;  //en=0, rs=0
    data_t[2] = data_l | 0x0D;  //en=1, rs=0
    data_t[3] = data_l | 0x09;  //en=0, rs=0

    //ESP_ERROR_CHECK(i2c_master_write_to_device(I2C_NUM_0, devAddr, data_t, 4, 1000));
}

void I2CLcd1602::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

}

void I2CLcd1602::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}
