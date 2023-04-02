//
// Created by Ivan Kishchenko on 21/03/2023.
//

#include "Database.h"
#include "DatabaseProperties.h"
#include "RegistryRepository.h"

namespace db {

    const char *Database::name() {
        return "database";
    }

    int Database::order() {
        return INT32_MAX - 1;
    }

    void Database::postConstruct(::Registry &registry) {
        auto props = registry.getProperties<DatabaseProperties>();
        _dataSource = std::make_unique<DataSource>(props.host, props.port, props.username, props.password, props.database);

        auto repository = createRepository<RegistryRepository>();
        auto res = repository.findById(1);
        info("{}:{}, {}", res.id, res.name, res.uuid);

        auto results = repository.findAll("", 0, 100);
        for (auto row: results) {
            info("{}:{}, {}", row.id, row.name, row.uuid);
        }

        BaseService::postConstruct(registry);
    }

    void Database::preDestroy(::Registry &registry) {
        BaseService::preDestroy(registry);
    }

}
