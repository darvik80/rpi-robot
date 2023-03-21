//
// Created by Ivan Kishchenko on 20/03/2023.
//

#pragma once

#include "Repository.h"
#include "DataSource.h"
#include <nlohmann/json.hpp>

namespace db {

    struct Registry {
        long id;
        std::string createdAt;
        std::string name;
        std::string uuid;
        int status;
        nlohmann::json json;
    };

    class RegistryRepository : public Repository {
        DataSource &_source;
    public:
        explicit RegistryRepository(DataSource &source) : _source(source) {}

        Registry getById(long regId);
        std::list<Registry> findAll(long offset, int size);

        long save(const Registry &registry);
        long update(const Registry &registry);
    };

}