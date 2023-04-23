//
// Created by Ivan Kishchenko on 21/03/2023.
//

#pragma once

#include <functional>
#include "db/DatabaseSupport.h"
#include "db/DataSource.h"
#include "DeviceDomainObjects.h"

class DeviceRepository : public TRepository<DeviceDo> {
public:
    explicit DeviceRepository(DataSource &source) : TRepository(source) {}

    Page<DeviceDo>::Ptr findAll(const Filter &filter, const PageRequest &page) override;

    long insert(const DeviceDo &model) override;

    void update(const DeviceDo &model) override;

    void deleteByFilter(const Filter &filter) override;
};
