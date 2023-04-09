//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>

#include "iot/IotPlatform.h"
#include "db/Database.h"
#include "http/HttpService.h"
#include "service/registry/RegistryRepository.h"
#include "service/device/DeviceRepository.h"

class RegistryDatabase : public Database {
public:
    void createRepositories() override {
        createRepository<RegistryRepository>();
        createRepository<DeviceRepository>();
    }
};

class RegistryApp : public Application {
public:
    const char *name() override {
        return "registry";
    }

protected:
    void setup(Registry &registry) override {
        registry.createService<IotRegistry>();
        registry.createService<RegistryDatabase>();
        registry.createService<HttpService>();
    }

    void destroy(Registry &registry) override {

    }
};


int main(int argc, char *argv[]) {

    RegistryApp app;
    app.run(argc, argv);

    return 0;
}