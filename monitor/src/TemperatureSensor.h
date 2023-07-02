//
// Created by Ivan Kishchenko on 02/07/2023.
//

#pragma once

#include <optional>

class TemperatureSensor {
public:
    enum Type {
        CPU,
        GPU,
        AMBIENT,
    };

    virtual std::optional<double> getTemperature(Type type);
};
