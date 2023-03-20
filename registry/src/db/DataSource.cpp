//
// Created by Ivan Kishchenko on 20/03/2023.
//

#include "DataSource.h"

namespace db {
    DataSource::DataSource(std::string_view host, int port, std::string_view username, std::string_view password, std::string_view database, int poolSize) {
        std::string connString = fmt::format("user={} host={} port={} password={} dbname={}", username, host, port, password, database);

        for (int idx = 0; idx < poolSize; ++idx) {
            _freeConn.emplace(std::make_shared<pqxx::connection>(connString));
        }
    }

    ConnectionPtr DataSource::getConnection() {
        const std::lock_guard _(_mutex);
        auto conn = _freeConn.top();
        _freeConn.pop();

        return conn;
    }

    void DataSource::releaseConnection(ConnectionPtr conn) {
        const std::lock_guard _(_mutex);
        _freeConn.push(conn);
    }

}
