//
// Created by Ivan Kishchenko on 02.08.2021.
//

#pragma once

#include "Joystick.h"

class XboxJoystick : public Joystick {
    JoystickAxis _axis[3];

    bool _rb{false};
    bool _lb{false};

    int _lt{0};
    int _rt{0};

    bool _btnA{false};
    bool _btnX{false};
    bool _btnB{false};
    bool _btnY{false};

    bool _btnBack{false};
    bool _btnXbox{false};
    bool _btnStart{false};
public:
    [[nodiscard]] const char *name() const override {
        return "xbox-joy";
    }

    [[nodiscard]] JoystickAxis getAxis(AxisId id) const {
        return _axis[id];
    }

    void setAxisX(AxisId id, int x) {
        _axis[id].x = x;
    }

    void setAxisY(AxisId id, int y) {
        _axis[id].y = y;
    }

    const JoystickAxis& getAxis(AxisId id) {
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

    [[nodiscard]] bool isBtnBack() const {
        return _btnY;
    }

    void setBtnBack(bool btnBack) {
        _btnBack = btnBack;
    }

    [[nodiscard]] bool isBtnXbox() const {
        return _btnY;
    }

    void setBtnXbox(bool btnXbox) {
        _btnXbox = btnXbox;
    }

    [[nodiscard]] bool isBtnStart() const {
        return _btnY;
    }

    void setBtnStart(bool btnStart) {
        _btnStart = btnStart;
    }
};
