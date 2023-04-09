//
// Created by Ivan Kishchenko on 20/03/2023.
//

#pragma once

#include "db/DatabaseSupport.h"
#include "db/DataSource.h"
#include <nlohmann/json.hpp>

struct RegistryDo {
    long id;
    std::string createdAt;
    std::string name;
    std::string uuid;
    int status;
    nlohmann::json json_data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RegistryDo, id, createdAt, name, uuid, status, json_data)

class RegistryRepository : public TRepository<RegistryDo> {
public:
    explicit RegistryRepository(DataSource &source) : TRepository(source) {}

    Page<RegistryDo>::Ptr findAll(const Filter &filter, const PageRequest &page) override;

    long insert(const RegistryDo &registry) override;;

    void update(const RegistryDo &registry) override;;
};
