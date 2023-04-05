//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include "SystemMonitorService.h"
#include "core-service/SchedulerService.h"
#include "core-service/EventBusService.h"
#include "iot/IotMessage.h"

#include <string>
#include <iomanip>
#include <utility>
#include <iostream>
#include <stdexcept>

void to_json(nlohmann::json &j, const SystemInfoEvent &e) {
    j = nlohmann::json{{"cpu-temp", e.cpuTemp}};
    if (e.gpuTemp) {
        j["gpu-temp"] = e.gpuTemp.value();
    }
}

void from_json(const nlohmann::json &j, SystemInfoEvent &e) {
    j.at("cpu-temp").get_to(e.cpuTemp);
    if (j.contains("gpu-temp")) {
        float gpuTemp;
        j.at("gpu-temp").get_to(gpuTemp);
        e.gpuTemp = gpuTemp;
    }
}

const char *SystemMonitorService::name() {
    return "monitor";
}

#ifdef __APPLE__

#include <IOKit/IOKitLib.h>

typedef char SMCBytes_t[32];
typedef char UInt32Char_t[5];

#define KERNEL_INDEX_SMC 2

#define SMC_KEY_CPU_TEMP "TC0P"
#define SMC_KEY_GPU_TEMP "TG0P"

#define SMC_CMD_READ_BYTES 5
#define SMC_CMD_READ_KEYINFO 9
#define DATATYPE_SP78 "sp78"

typedef struct {
    char major;
    char minor;
    char build;
    char reserved[1];
    UInt16 release;
} SMCKeyData_vers_t;

typedef struct {
    UInt16 version;
    UInt16 length;
    UInt32 cpuPLimit;
    UInt32 gpuPLimit;
    UInt32 memPLimit;
} SMCKeyData_pLimitData_t;

typedef struct {
    UInt32 dataSize;
    UInt32 dataType;
    char dataAttributes;
} SMCKeyData_keyInfo_t;

typedef struct {
    UInt32Char_t key;
    UInt32 dataSize;
    UInt32Char_t dataType;
    SMCBytes_t bytes;
} SMCVal_t;

typedef struct {
    UInt32 key;
    SMCKeyData_vers_t vers;
    SMCKeyData_pLimitData_t pLimitData;
    SMCKeyData_keyInfo_t keyInfo;
    char result;
    char status;
    char data8;
    UInt32 data32;
    SMCBytes_t bytes;
} SMCKeyData_t;

static io_connect_t conn;

UInt32 StrToULong(const char *str, int size, int base) {
    UInt32 total = 0;
    int i;

    for (i = 0; i < size; i++) {
        if (base == 16)
            total += str[i] << (size - 1 - i) * 8;
        else
            total += (unsigned char) (str[i] << (size - 1 - i) * 8);
    }
    return total;
}

void ULongToStr(char *str, UInt32 val) {
    str[0] = '\0';
    snprintf(str, 5, "%c%c%c%c",
            (unsigned int) val >> 24,
            (unsigned int) val >> 16,
            (unsigned int) val >> 8,
            (unsigned int) val);
}

kern_return_t SMCCall(int index, SMCKeyData_t *inputStructure, SMCKeyData_t *outputStructure) {
    size_t structureInputSize;
    size_t structureOutputSize;

    structureInputSize = sizeof(SMCKeyData_t);
    structureOutputSize = sizeof(SMCKeyData_t);

#if MAC_OS_X_VERSION_10_5
    return IOConnectCallStructMethod(
            conn, index,
            inputStructure, structureInputSize,
            outputStructure, &structureOutputSize
    );
#else
    return IOConnectMethodStructureIStructureO(conn, index,
        structureInputSize, /* structureInputSize */
        &structureOutputSize, /* structureOutputSize */
        inputStructure, /* inputStructure */
        outputStructure); /* ouputStructure */
#endif
}

kern_return_t SMCOpen() {
    kern_return_t result;
    mach_port_t masterPort;
    io_iterator_t iterator;
    io_object_t device;

    result = IOMasterPort(MACH_PORT_NULL, &masterPort);

    CFMutableDictionaryRef matchingDictionary = IOServiceMatching("AppleSMC");
    result = IOServiceGetMatchingServices(masterPort, matchingDictionary, &iterator);
    if (result != kIOReturnSuccess) {
        printf("Error: IOServiceGetMatchingServices() = %08x\n", result);
        return 1;
    }

    device = IOIteratorNext(iterator);
    IOObjectRelease(iterator);
    if (device == 0) {
        printf("Error: no SMC found\n");
        return 1;
    }

    result = IOServiceOpen(device, mach_task_self(), 0, &conn);
    IOObjectRelease(device);
    if (result != kIOReturnSuccess) {
        printf("Error: IOServiceOpen() = %08x\n", result);
        return 1;
    }

    return kIOReturnSuccess;
}

kern_return_t SMCClose() {
    return IOServiceClose(conn);
}

kern_return_t SMCReadKey(UInt32Char_t key, SMCVal_t *val) {
    kern_return_t result;
    SMCKeyData_t inputStructure;
    SMCKeyData_t outputStructure;

    memset(&inputStructure, 0, sizeof(SMCKeyData_t));
    memset(&outputStructure, 0, sizeof(SMCKeyData_t));
    memset(val, 0, sizeof(SMCVal_t));

    inputStructure.key = StrToULong(key, 4, 16);
    inputStructure.data8 = SMC_CMD_READ_KEYINFO;

    result = SMCCall(KERNEL_INDEX_SMC, &inputStructure, &outputStructure);
    if (result != kIOReturnSuccess)
        return result;

    val->dataSize = outputStructure.keyInfo.dataSize;
    ULongToStr(val->dataType, outputStructure.keyInfo.dataType);
    inputStructure.keyInfo.dataSize = val->dataSize;
    inputStructure.data8 = SMC_CMD_READ_BYTES;

    result = SMCCall(KERNEL_INDEX_SMC, &inputStructure, &outputStructure);
    if (result != kIOReturnSuccess)
        return result;

    memcpy(val->bytes, outputStructure.bytes, sizeof(outputStructure.bytes));

    return kIOReturnSuccess;
}

float SMCGetTemperature(char *key) {
    SMCVal_t val;
    kern_return_t result;

    result = SMCReadKey(key, &val);
    if (result == kIOReturnSuccess) {
        // read succeeded - check returned value
        if (val.dataSize > 0) {
            if (strcmp(val.dataType, DATATYPE_SP78) == 0) {
                // convert sp78 value to temperature
                int intValue = val.bytes[0] * 256 + (unsigned char) val.bytes[1];
                return intValue / 256.0;
            }
        }
    }
    // read failed
    return 0.0;
}

#else

double SMCGetTemperature(char *key) {
    return 0.0;
}

#endif

void SystemMonitorService::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    auto eventService = registry.getService<EventBusService>().shared_from_this();

//    eventService->subscribe<IotCommand>([this](const IotCommand &event) -> bool {
//        info("handle cmd: {}", event.name);
//        return true;
//    });

    registry.getService<SchedulerService>().scheduleAtFixedRate(
            [this, eventService]() -> void {
#ifdef __APPLE__
                SMCOpen();
                nlohmann::json json = {
                        {"cpu-temp", SMCGetTemperature(SMC_KEY_CPU_TEMP)},
                        {"gpu-temp", SMCGetTemperature(SMC_KEY_GPU_TEMP)},
                };
                SMCClose();
                logger::info("data: {}", json.dump());
                eventService->send(IotTelemetry{0, json.dump()});

#else
                std::ifstream piCpuTempFile;
                std::stringstream buffer;
                piCpuTempFile.open("/sys/class/thermal/thermal_zone0/temp");
                nlohmann::json json;
                if (piCpuTempFile.is_open()) {
                    buffer << piCpuTempFile.rdbuf();
                    piCpuTempFile.close();

                    json = {
                            {"cpu-temp", round(std::stof(buffer.str()) / 1000 * 100) / 100}

                    };

                } else {
                    json = {
                            {"cpu-temp", "no-data"}

                    };
                }
                logger::info("data: {}", json.dump());
                eventService->send(IotTelemetry{"monitor", json.dump()});
#endif
            },
            boost::posix_time::seconds{0},
            boost::posix_time::seconds{5}
    );
}

void SystemMonitorService::preDestroy(Registry &registry) {
    BaseService::preDestroy(registry);
}

