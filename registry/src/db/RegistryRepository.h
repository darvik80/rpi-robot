//
// Created by Ivan Kishchenko on 20/03/2023.
//

#pragma once

#include "Repository.h"
#include "DataSource.h"
#include <nlohmann/json.hpp>

struct IotRegistry {
    long id;
    std::string createdAt;
    std::string name;
    std::string uuid;
    int status;
    nlohmann::json json_data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IotRegistry, id, createdAt, name, uuid, status, json_data)

class RegistryRepository : public Repository {
    DataSource &_source;
public:
    explicit RegistryRepository(DataSource &source) : _source(source) {}

    IotRegistry findById(long regId);

    std::optional<IotRegistry> findByName(std::string_view name);

    std::list<IotRegistry> findAll(std::string_view name, long offset, int size);
    size_t findAllCount(std::string_view name);

    long insert(const IotRegistry &registry);

    void update(const IotRegistry &registry);
};
