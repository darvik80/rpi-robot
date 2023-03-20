//
// Created by Ivan Kishchenko on 20/03/2023.
//

#pragma once

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

    class RegistryRepository {
        DataSource::Ptr _source;
    public:
        explicit RegistryRepository(const DataSource::Ptr &source) : _source(source) {}

        Registry getById(long regId);

        long save(const Registry& registry);
    };

}