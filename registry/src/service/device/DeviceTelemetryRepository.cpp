//
// Created by Ivan Kishchenko on 11/04/2023.
//

#include "DeviceTelemetryRepository.h"
#include <unordered_set>

Page<DeviceTelemetryDo>::Ptr DeviceTelemetryRepository::findAll(const Filter &filter, const PageRequest &page) {
    auto result = std::make_shared<Page<DeviceTelemetryDo>>();

    sql::SelectModel s;
    applyFilter(s, filter);
    s.select("id", "device_id", "created_at", "json_data")
            .from("device_telemetry")
            .offset(page.offset)
            .limit(page.size)
            .order_by("id DESC");

    pqxx::nontransaction w(_source.getConnection());
    auto res = w.exec_params(s.str());

    std::unordered_set<long> deviceIds;
    for (const auto &row: res) {
        result->data.push_back(
                {
                        .id = row.at(0).as<long>(),
                        .deviceId = row.at(1).as<long>(),
                        .createdAt = row.at(2).as<std::string>(),
                        .json_data = nlohmann::json::parse(row.at(3).as<std::string>()),
                }
        );
        deviceIds.insert(result->data.back().deviceId);
    }

    if (!deviceIds.empty()) {
        sql::SelectModel cs;
        cs.select("id", "created_at", "name", "uuid", "status", "json_data")
                .from("device")
                .where(sql::column("id").in(deviceIds));

        auto dev = w.exec_params(cs.str());
        for (const auto &row: dev) {
            DeviceDo device = {
                    .id = row.at(0).as<long>(),
                    .createdAt = row.at(1).as<std::string>(),
                    .name = row.at(2).as<std::string>(),
                    .uuid = row.at(3).as<std::string>(),
                    .status = row.at(4).as<int>(),
                    .json_data = nlohmann::json::parse(row.at(5).as<std::string>()),
            };

            for (auto &rec: result->data) {
                if (rec.deviceId == device.id) {
                    rec.refDevice = device;
                }
            }
        }
    }


    if (page.hasCount) {
        sql::SelectModel sc;
        applyFilter(sc, filter);
        sc.select("COUNT(id)").from("device_telemetry");
        result->total = w.exec_params1(sc.str()).at(0).as<size_t>();;
    }

    return result;
}

long DeviceTelemetryRepository::insert(const DeviceTelemetryDo &model) {
    pqxx::work worker(_source.getConnection());

    sql::InsertModel i;
    i.insert("device_id", model.deviceId)
                    ("json_data", model.json_data.dump())
            .into("device_telemetry")
            .returning("id");
    auto res = worker.exec_params1(i.str());
    worker.commit();

    return res[0].as<long>();
}

void DeviceTelemetryRepository::update(const DeviceTelemetryDo &model) {
}


void DeviceTelemetryRepository::remove(long id) {
    pqxx::work worker(_source.getConnection());
    sql::DeleteModel d;
    d.from("device_telemetry").where(sql::column("id") == id);

    auto res = worker.exec_params(d.str());
    worker.commit();
}