//
// Created by Ivan Kishchenko on 20/03/2023.
//

#include "DataSource.h"

namespace db {
    DataSource::DataSource(std::string_view host, int port, std::string_view username, std::string_view password, std::string_view database) {
        _connString = fmt::format("user={} host={} port={} password={} dbname={}", username, host, port, password, database);

        _cache.emplace(std::this_thread::get_id(), _connString);
    }

    pqxx::connection& DataSource::getConnection() {
        const std::lock_guard _(_mutex);
        if (auto it = _cache.find(std::this_thread::get_id()); it != std::end(_cache)) {
            return it->second;
        };

        return _cache.emplace(std::this_thread::get_id(), _connString).first->second;
    }
}
