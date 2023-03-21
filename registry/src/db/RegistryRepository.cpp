//
// Created by Ivan Kishchenko on 20/03/2023.
//

#include "RegistryRepository.h"
#include "SqlBuilder.h"

namespace db {
    Registry RegistryRepository::getById(long regId) {
        pqxx::nontransaction w(_source.getConnection());

        sql::SelectModel s;
        s.select("id", "created_at", "name", "uuid", "status", "json_data")
                .from("registry")
                .where(sql::column("id") == regId);

        auto row = w.exec_params1(s.str());

        return Registry{
                .id = row.at(0).as<long>(),
                .createdAt = row.at(1).as<std::string>(),
                .name = row.at(2).as<std::string>(),
                .uuid = row.at(3).as<std::string>(),
                .status = row.at(4).as<int>(),
                .json = row.at(5).as<std::string>(),
        };
    }

    std::list<Registry> RegistryRepository::findAll(long offset, int limit) {
        pqxx::nontransaction w(_source.getConnection());

        sql::SelectModel s;
        s.select("id", "created_at", "name", "uuid", "status", "json_data")
                .from("registry")
                .offset(offset)
                .limit(limit)
                .order_by("id");

        auto res = w.exec_params(s.str());

        std::list<Registry> result;
        for (const auto &row: res) {
            result.push_back(
                    {
                            .id = row.at(0).as<long>(),
                            .createdAt = row.at(1).as<std::string>(),
                            .name = row.at(2).as<std::string>(),
                            .uuid = row.at(3).as<std::string>(),
                            .status = row.at(4).as<int>(),
                            .json = row.at(5).as<std::string>(),
                    }
            );
        }

        return result;
    }

    long RegistryRepository::save(const Registry &registry) {
        pqxx::work worker(_source.getConnection());

        auto res = worker.exec_params1(
                "INSERT INTO registry (created_at, name, uuid, json_data) VALUES (NOW(), $1, gen_random_uuid(), $2) RETURNING id;",
                registry.name, registry.json.dump()
        );
        worker.commit();

        return res[0].as<long>();
    }

}