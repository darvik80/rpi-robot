//
// Created by Ivan Kishchenko on 02/04/2023.
//

#pragma once

#include "service/RestRepositoryController.h"
#include "RegistryRepository.h"

class RegistryRestController : public DatabaseRestRepositoryController<RegistryDo, RegistryRepository> {
public:
    explicit RegistryRestController(Database &database) : DatabaseRestRepositoryController(database) {}
};
