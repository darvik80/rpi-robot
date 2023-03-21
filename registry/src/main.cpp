//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>

#include "iot/IotProperties.h"
#include "iot/IotPlatform.h"

#include "db/Database.h"
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
        registry.createService<IotRegistry>();
        registry.createService<db::Database>();
    }

    void destroy(Registry &registry) override {

    }
};

int main(int argc, char *argv[]) {
    RegistryApp app;
    app.run(argc, argv);

    return 0;
}