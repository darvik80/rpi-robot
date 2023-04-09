//
// Created by Ivan Kishchenko on 21/03/2023.
//

#include "Database.h"
#include "DatabaseProperties.h"
#include "service/registry/RegistryRepository.h"

const char *Database::name() {
    return "database";
}

int Database::order() {
    return INT32_MAX - 1;
}

void Database::postConstruct(::Registry &registry) {
    auto props = registry.getProperties<DatabaseProperties>();
    _dataSource = std::make_unique<DataSource>(props.host, props.port, props.username, props.password, props.database);

    createRepositories();
    BaseService::postConstruct(registry);
}

void Database::preDestroy(::Registry &registry) {
    BaseService::preDestroy(registry);
}
