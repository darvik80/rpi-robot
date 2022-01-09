//
// Created by Ivan Kishchenko on 02.08.2021.
//

#pragma once

#include "Joystick.h"

enum XboxAxisId {
    axis_left,
    axis_mid,
    axis_right,
    axis_size
};

class XboxJoystick : public Joystick {
    JoystickAxis _axis[axis_size];

    bool _rb{false};
    bool _lb{false};

    int _lt{0};
    int _rt{0};

    bool _btnA{false};
    bool _btnX{false};
    bool _btnB{false};
    bool _btnY{false};
public:
    XboxJoystick()
            : Joystick("xbox360") { }

    [[nodiscard]] JoystickAxis getAxis(XboxAxisId id) const {
        return _axis[id];
    }

    void setAxisX(XboxAxisId id, int x) {
        _axis[id].x = x;
    }

    void setAxisY(XboxAxisId id, int y) {
        _axis[id].y = y;
    }

    const JoystickAxis& getAxis(XboxAxisId id) {
        return _axis[id];
    }

    [[nodiscard]] bool isRb() const {
        return _rb;
    }

    void setRb(bool rb) {
        _rb = rb;
    }

    [[nodiscard]] bool isLb() const {
        return _lb;
    }

    void setLb(bool lb) {
        _lb = lb;
    }

    [[nodiscard]] int getLt() const {
        return _lt;
    }

    void setLt(int lt) {
        _lt = lt;
    }

    [[nodiscard]] int getRt() const {
        return _rt;
    }

    void setRt(int rt) {
        _rt = rt;
    }

    [[nodiscard]] bool isBtnA() const {
        return _btnA;
    }

    void setBtnA(bool btnA) {
        _btnA = btnA;
    }

    [[nodiscard]] bool isBtnX() const {
        return _btnX;
    }

    void setBtnX(bool btnX) {
        _btnX = btnX;
    }

    [[nodiscard]] bool isBtnB() const {
        return _btnB;
    }

    void setBtnB(bool btnB) {
        _btnB = btnB;
    }

    [[nodiscard]] bool isBtnY() const {
        return _btnY;
    }

    void setBtnY(bool btnY) {
        _btnY = btnY;
    }
};
