//
// Created by Ivan Kishchenko on 20/03/2023.
//

#pragma once

#include <fmt/format.h>
#include <pqxx/pqxx>
#include <unordered_map>
#include <thread>
#include <mutex>


class DataSource {
    std::string _connString;
    std::mutex _mutex;
    std::unordered_map<std::thread::id, pqxx::connection> _cache;

public:
    typedef std::unique_ptr<DataSource> Ptr;

    DataSource(std::string_view host, int port, std::string_view username, std::string_view password, std::string_view database);

    pqxx::connection& getConnection();
};
