//
// Created by Ivan Kishchenko on 20/03/2023.
//

#include "RegistryRepository.h"

namespace db {
    Registry RegistryRepository::getById(long regId) {
        ConnGuard guard(*_source);
        auto &conn = *guard;
        pqxx::nontransaction w(conn);

        auto [id, createdAt, name, uuid, status, json] = w.exec_params1(
                "SELECT id, created_at, name, uuid, json_data FROM registry WHERE id = $1", regId
        ).as<long, std::string, std::string, std::string, int, std::string>();

        return Registry{
                .id = id,
                .createdAt = createdAt,
                .name = name,
                .uuid = uuid,
                .status = status,
                .json = json,
        };
    }

    long RegistryRepository::save(const Registry &registry) {
        ConnGuard guard(*_source);
        pqxx::work worker(*guard);

        auto res = worker.exec_params1(
                "INSERT INTO registry (created_at, name, uuid, json_data) VALUES (NOW(), $1, gen_random_uuid(), $2) RETURNING id;",
                registry.name, registry.json.dump()
        );
        worker.commit();

        return res[0].as<long>();
    }
}