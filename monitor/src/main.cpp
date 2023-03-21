//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>
#include "SystemMonitorService.h"

#include <iostream>

class Monitor : public Application {
public:
    const char *name() override {
        return "monitor";
    }

protected:
    void setup(Registry &registry) override {
        registry.addService(std::make_shared<SystemMonitorService>());
        //registry.addService(std::make_shared<IotPlatformService>());
    }

    void destroy(Registry &registry) override {

    }
};

int main(int argc, char *argv[]) {
    Monitor app;
    app.run(argc, argv);

    return 0;
}