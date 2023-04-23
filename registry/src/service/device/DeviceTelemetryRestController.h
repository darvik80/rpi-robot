//
// Created by Ivan Kishchenko on 11/04/2023.
//

#pragma once

#include "service/RestRepositoryController.h"
#include "DeviceTelemetryRepository.h"

class DeviceTelemetryRestController : public DatabaseRestRepositoryController<DeviceTelemetryDo, DeviceTelemetryRepository> {
public:
    explicit DeviceTelemetryRestController(Database &database) : DatabaseRestRepositoryController(database) {}
};
