//
// Created by Ivan Kishchenko on 20/03/2023.
//

#pragma once

#include <fmt/format.h>
#include <pqxx/pqxx>
#include <unordered_set>

namespace db {
    typedef std::shared_ptr<pqxx::connection> ConnectionPtr;

    class DataSource {

        std::mutex _mutex;
        std::stack<ConnectionPtr> _freeConn;

        friend class ConnGuard;

    public:
        typedef std::shared_ptr<DataSource> Ptr;

        DataSource(std::string_view host, int port, std::string_view username, std::string_view password, std::string_view database, int poolSize = 4);

    private:
        ConnectionPtr getConnection();

        void releaseConnection(ConnectionPtr conn);
    };

    class ConnGuard {
        DataSource &_source;
        ConnectionPtr _conn;
    public:
        explicit ConnGuard(DataSource &source) : _source(source), _conn(_source.getConnection()) {

        }

        pqxx::connection &operator*() {
            return *_conn;
        }

        ~ConnGuard() {
            _source.releaseConnection(_conn);
        }
    };

}