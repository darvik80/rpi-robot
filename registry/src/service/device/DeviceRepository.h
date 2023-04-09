//
// Created by Ivan Kishchenko on 21/03/2023.
//

#pragma once

#include "db/DatabaseSupport.h"
#include "db/DataSource.h"
#include <nlohmann/json.hpp>
#include <functional>

struct DeviceDo {
    long id;
    std::string createdAt;
    std::string updatedAt;
    std::string name;
    std::string uuid;
    int status;
    nlohmann::json json_data;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeviceDo, id, createdAt, name, uuid, status, json_data)

class DeviceRepository : public TRepository<DeviceDo> {
public:
    explicit DeviceRepository(DataSource &source) : TRepository(source) {}

    Page<DeviceDo>::Ptr findAll(const Filter &filter, const PageRequest &page) override;

    long insert(const DeviceDo &model) override;

    void update(const DeviceDo &model) override;
};
