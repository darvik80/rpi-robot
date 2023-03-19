//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>

#include "iot/IotProperties.h"
#include "iot/IotPlatform.h"

class RegistryApp : public Application {
public:
    const char *name() override {
        return "registry";
    }

protected:
    void setup(Registry &registry) override {
        auto props = registry.getProperties<IotProperties>();
        registry.createService<IotRegistry>(props.registryId);
    }

    void destroy(Registry &registry) override {

    }
};

int main(int argc, char *argv[]) {
    RegistryApp app;
    app.run(argc, argv);

    return 0;
}