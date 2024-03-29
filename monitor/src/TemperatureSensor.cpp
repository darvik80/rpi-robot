//
// Created by Ivan Kishchenko on 02/07/2023.
//

#include "TemperatureSensor.h"

#ifdef __APPLE__
#include "macos/smc.h"
#else

#include <string>
#include <iomanip>
#include <cmath>
#include <fstream>

#endif

std::optional<double> TemperatureSensor::getTemperature(TemperatureSensor::Type type) {
    std::optional<double> result;
    
#ifdef __APPLE__
    SMCOpen();

    switch (type) {
        case CPU:
            result = SMCGetTemperature(SMC_KEY_CPU_TEMP);
            break;
        case GPU:
            result = SMCGetTemperature(SMC_KEY_GPU_TEMP);
            break;
        case AMBIENT:
            result = SMCGetTemperature(SMC_KEY_AMBIENT_TEMP);
            break;
    }

#else
    switch (type) {
        case CPU: {
            std::ifstream piCpuTempFile;
            std::stringstream buffer;
            piCpuTempFile.open("/sys/class/thermal/thermal_zone0/temp");
            if (piCpuTempFile.is_open()) {
                buffer << piCpuTempFile.rdbuf();
                piCpuTempFile.close();
                result = round(std::stod(buffer.str()) / 1000 * 100) / 100;
            }
        }
        default:
            break;
    }
#endif

    return result;
}