//
// Created by Ivan Kishchenko on 01.08.2021.
//

#include "JoystickEvent.h"

namespace xbox {
    void to_json(nlohmann::json &j, const Xbox380Event &event) {
        j = {
                {"rb",         event.isRb()},
                {"lb",         event.isLb()},
                {"rt",         event.getRt()},
                {"lt",         event.getLt()},
                {"btn-a",      event.isBtnA()},
                {"btn-x",      event.isBtnX()},
                {"btn-b",      event.isBtnB()},
                {"btn-Y",      event.isBtnY()},
                {"left-axis",  {{"x", event.getAxis(axis_left).x},  {"y", event.getAxis(axis_left).y}}},
                {"mid-axis",   {{"x", event.getAxis(axis_mid).x},   {"y", event.getAxis(axis_mid).y}}},
                {"right-axis", {{"x", event.getAxis(axis_right).x}, {"y", event.getAxis(axis_right).y}}}
        };
    }

    void from_json(const nlohmann::json &j, Xbox380Event &event) {
        event.setRb(j.at("rb").get<bool>());
        event.setLb(j.at("lb").get<bool>());

        event.setRb(j.at("rt").get<int>());
        event.setLb(j.at("lt").get<int>());

        event.setBtnA(j.at("btn-a").get<bool>());
        event.setBtnX(j.at("btn-x").get<bool>());
        event.setBtnB(j.at("btn-b").get<bool>());
        event.setBtnY(j.at("btn-y").get<bool>());

        auto axis = j.at("left-axis");
        event.setAxisX(axis_left, axis.at("x").get<int>());
        event.setAxisY(axis_left, axis.at("y").get<int>());

        axis = j.at("right-axis");
        event.setAxisX(axis_right, axis.at("x").get<int>());
        event.setAxisY(axis_right, axis.at("y").get<int>());

        axis = j.at("mid-axis");
        event.setAxisX(axis_mid, axis.at("x").get<int>());
        event.setAxisY(axis_mid, axis.at("y").get<int>());
    }

}