//
// Created by Ivan Kishchenko on 20/03/2023.
//

#include "RegistryRepository.h"
#include "SqlBuilder.h"

namespace db {
    Registry RegistryRepository::findById(long regId) {
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
                .json_data = nlohmann::json::parse(row.at(5).as<std::string>()),
        };
    }

    std::optional<Registry> RegistryRepository::findByName(std::string_view name) {
        pqxx::nontransaction w(_source.getConnection());

        sql::SelectModel s;
        s.select("id", "created_at", "name", "uuid", "status", "json_data")
                .from("registry")
                .where(sql::column("name").like(name));

        auto row = w.exec_params1(s.str());

        return Registry{
                .id = row.at(0).as<long>(),
                .createdAt = row.at(1).as<std::string>(),
                .name = row.at(2).as<std::string>(),
                .uuid = row.at(3).as<std::string>(),
                .status = row.at(4).as<int>(),
                .json_data = nlohmann::json::parse(row.at(5).as<std::string>()),
        };
    }

    std::list<Registry> RegistryRepository:: findAll(std::string_view name, long offset, int limit) {
        pqxx::nontransaction w(_source.getConnection());

        sql::SelectModel s;
        s.select("id", "created_at", "name", "uuid", "status", "json_data")
                .from("registry")
                .offset(offset)
                .limit(limit)
                .order_by("id");

        if (!name.empty()) {
            s.where(sql::column("name").like(name));
        }

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
                            .json_data = nlohmann::json::parse(row.at(5).as<std::string>()),
                    }
            );
        }

        return result;
    }

    size_t RegistryRepository::findAllCount(std::string_view name) {
        pqxx::nontransaction w(_source.getConnection());

        sql::SelectModel s;
        s.select("COUNT(id)").from("registry");

        if (!name.empty()) {
            s.where(sql::column("name").like(name));
        }

        return w.exec_params1(s.str()).at(0).as<size_t>();
    }

    long RegistryRepository::insert(const Registry &registry) {
        pqxx::work worker(_source.getConnection());

        sql::InsertModel i;
        i.insert("name", registry.name)
                        ("json_data", registry.json_data.dump())
                        ("uuid", sql::func("gen_random_uuid()"))
                        ("updated_at", sql::func("now()"))
                .into("registry")
                .returning("id");
        auto res = worker.exec_params1(i.str());
        worker.commit();

        return res[0].as<long>();
    }

    void RegistryRepository::update(const Registry &registry) {
        pqxx::work worker(_source.getConnection());

        sql::UpdateModel i;
        i.update("registry")
                .set("name", registry.name)
                        ("createdAt", registry.createdAt)
                        ("status", registry.status)
                        ("uuid", registry.uuid)
                        ("json_data", registry.json_data.dump())
                        .where(sql::column("id") == registry.id);
        auto res = worker.exec_params1(i.str());
        worker.commit();
    }

}