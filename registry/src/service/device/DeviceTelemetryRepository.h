//
// Created by Ivan Kishchenko on 11/04/2023.
//

#pragma once

#include "DeviceDomainObjects.h"
#include "db/DatabaseSupport.h"
#include "db/DataSource.h"


class DeviceTelemetryRepository : public TRepository<DeviceTelemetryDo> {
    public:
        explicit DeviceTelemetryRepository(DataSource &source) : TRepository(source) {}

        Page<DeviceTelemetryDo>::Ptr findAll(const Filter &filter, const PageRequest &page) override;

        long insert(const DeviceTelemetryDo &model) override;

        void update(const DeviceTelemetryDo &model) override;

};
