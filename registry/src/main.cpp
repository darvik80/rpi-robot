//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>

#include "iot/IotPlatform.h"

#include "db/Database.h"
#include "db/RegistryRepository.h"

#include "http/HttpService.h"

#include <pqxx/pqxx>

class RegistryApp : public Application {
public:
    const char *name() override {
        return "registry";
    }

protected:
    void setup(Registry &registry) override {
        registry.createService<IotRegistry>();
        registry.createService<db::Database>();
        registry.createService<HttpService>();


        std::vector<std::string> data{"1", "2", "3"};

        std::any variant = data;

        if (variant.type() == typeid(std::vector<std::string>)) {
            info("any type: {}", variant.type().name());
        }

        variant = std::string("hello");
        info("any type: {}", variant.type().name());

        variant = 1;
        info("any type: {}", variant.type().name());
    }

    void destroy(Registry &registry) override {

    }
};

int main(int argc, char *argv[]) {
    RegistryApp app;
    app.run(argc, argv);

    return 0;
}