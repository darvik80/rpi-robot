//
// Created by Ivan Kishchenko on 21/03/2023.
//

#pragma once

#include "Repository.h"
#include "DataSource.h"
#include <nlohmann/json.hpp>

namespace db {

    struct Device {
        long id;
        std::string createdAt;
        std::string updatedAt;
        std::string name;
        std::string uuid;
        int status;
        nlohmann::json json;
    };

    class DeviceRepository : public Repository {

    };

}