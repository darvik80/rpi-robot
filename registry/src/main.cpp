//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>

#include "iot/IotProperties.h"
#include "iot/IotPlatform.h"
#include "db/DataSource.h"
#include "db/RegistryRepository.h"

#include <pqxx/pqxx>

struct device {
    long id;
    std::string name;
    std::string uuid;
    nlohmann::json jsonData;
};

class RegistryApp : public Application {
public:
    const char *name() override {
        return "registry";
    }

protected:
    void setup(Registry &registry) override {
        auto props = registry.getProperties<IotProperties>();
        registry.createService<IotRegistry>(props.registryId);


        pqxx::connection conn(
                "user=postgres "
                "host=127.0.0.1 "
                "port=5432 "
                "password=1234 "
                "dbname=postgres"
        );

        {
            pqxx::work w(conn);
            auto r = w.exec("SELECT id, name, uuid, json_data FROM device");

            auto [devId, name, uuid, json] = r.begin().as<int, std::string, std::string, std::string>();

            info("{}:{}, {} {}", devId, name, uuid, json);
            w.commit();
        }

        auto source = std::make_shared<db::DataSource>("127.0.0.1", 5432, "postgres", "1234", "postgres");
        db::RegistryRepository repository(source);
        auto res = repository.getById(1);
        info("{}:{}, {}", res.id, res.name, res.uuid);

        db::Registry reg{
                .name = "crearts-data-dup1",
                .json = {
                        {"status", "offline"}
                }
        };
        info("new rec: {}", repository.save(reg));
    }

    void destroy(Registry &registry) override {

    }
};

int main(int argc, char *argv[]) {
    RegistryApp app;
    app.run(argc, argv);

    return 0;
}