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

        Registry findById(long regId);
        std::optional<Registry> findByName(std::string_view name);
        std::list<Registry> findAll(long offset, int size);

        long insert(const Registry &registry);
        void update(const Registry &registry);
    };

}