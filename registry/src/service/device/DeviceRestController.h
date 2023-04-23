//
// Created by Ivan Kishchenko on 02/04/2023.
//

#pragma once

#include "service/RestRepositoryController.h"
#include "DeviceRepository.h"

class DeviceRestController : public DatabaseRestRepositoryController<DeviceDo, DeviceRepository> {
public:
    explicit DeviceRestController(Database &database) : DatabaseRestRepositoryController(database) {}
};
