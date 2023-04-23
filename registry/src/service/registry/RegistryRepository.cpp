//
// Created by Ivan Kishchenko on 20/03/2023.
//

#include "RegistryRepository.h"
#include "db/SqlBuilder.h"

Page<RegistryDo>::Ptr RegistryRepository::findAll(const Filter &filter, const PageRequest &page) {
    auto result = std::make_shared<Page<RegistryDo>>();

    sql::SelectModel s;
    applyFilter(s, filter);
    auto sdata{s};
    sdata.select("id", "created_at", "name", "uuid", "status", "json_data")
            .from("registry")
            .offset(page.offset)
            .limit(page.size)
            .order_by("id");

    pqxx::nontransaction w(_source.getConnection());
    auto res = w.exec_params(sdata.str());

    std::list<RegistryDo> test;
    for (const auto &row: res) {
        result->data.push_back(
                {
                        .id = row.at("id").as<long>(),
                        .createdAt = row.at(1).as<std::string>(),
                        .name = row.at(2).as<std::string>(),
                        .uuid = row.at(3).as<std::string>(),
                        .status = row.at(4).as<int>(),
                        .json_data = nlohmann::json::parse(row.at(5).as<std::string>()),
                }
        );
    }

    if (page.hasCount) {
        auto scount{s};
        scount.select("COUNT(id)").from("registry");
        result->total = w.exec_params1(scount.str()).at(0).as<size_t>();;
    }

    return result;
}

long RegistryRepository::insert(const RegistryDo &registry) {
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

void RegistryRepository::update(const RegistryDo &registry) {
    pqxx::work worker(_source.getConnection());

    sql::UpdateModel i;
    i.update("registry")
            .set("name", registry.name)
                    ("status", registry.status)
                    ("json_data", registry.json_data.dump())
            .where(sql::column("id") == registry.id);
    auto res = worker.exec_params(i.str());
    worker.commit();
}

void RegistryRepository::deleteByFilter(const Filter &filter) {
    pqxx::work worker(_source.getConnection());

    sql::DeleteModel d;
    d.from("registry");
    applyFilter<sql::DeleteModel>(d, filter);
    auto res = worker.exec_params(d.str());
    worker.commit();
}
