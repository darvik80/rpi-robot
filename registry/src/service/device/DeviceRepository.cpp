//
// Created by Ivan Kishchenko on 21/03/2023.
//

#include "DeviceRepository.h"

Page<DeviceDo>::Ptr DeviceRepository::findAll(const Filter &filter, const PageRequest &page) {
    auto result = std::make_shared<Page<DeviceDo>>();

    sql::SelectModel s;
    applyFilter(s, filter);
    auto sdata{s};
    sdata.select("id", "created_at", "name", "uuid", "status", "json_data")
            .from("device")
            .offset(page.offset)
            .limit(page.size)
            .order_by("id");

    pqxx::nontransaction w(_source.getConnection());
    auto res = w.exec_params(sdata.str());

    std::list<DeviceDo> test;
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
        scount.select("COUNT(id)").from("device");
        result->total = w.exec_params1(scount.str()).at(0).as<size_t>();;
    }

    return result;
}

long DeviceRepository::insert(const DeviceDo &model) {
    pqxx::work worker(_source.getConnection());

    sql::InsertModel i;
    i.insert("name", model.name)
                    ("json_data", model.json_data.dump())
                    ("uuid", sql::func("gen_random_uuid()"))
                    ("updated_at", sql::func("now()"))
            .into("registry")
            .returning("id");
    auto res = worker.exec_params1(i.str());
    worker.commit();

    return res[0].as<long>();
}

void DeviceRepository::update(const DeviceDo &model) {
    pqxx::work worker(_source.getConnection());

    sql::UpdateModel i;
    i.update("registry")
            .set("name", model.name)
                    ("createdAt", model.createdAt)
                    ("status", model.status)
                    ("uuid", model.uuid)
                    ("json_data", model.json_data.dump())
            .where(sql::column("id") == model.id);
    auto res = worker.exec_params1(i.str());
    worker.commit();
}

