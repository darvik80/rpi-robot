//
// Created by Ivan Kishchenko on 25.02.2022.
//

#include <Application.h>
#include "SystemMonitorService.h"
#include "IoTService.h"

#include <iostream>

class Monitor : public Application {
public:
    const char *name() override {
        return "monitor";
    }

protected:
    void setup(Registry &registry) override {
        registry.addService(std::make_shared<SystemMonitorService>());
        registry.addService(std::make_shared<IoTService<network::SslSocket>>());
    }

    void destroy(Registry &registry) override {

    }
};

bool compareTopics(std::string_view topic, std::string_view origin) {
    enum class State {
        None,
        Slash,
    } state = State::None;
    auto it = topic.begin(), oit = origin.begin();

    while (it != topic.end() && oit != origin.end()) {
        switch (state) {
            case State::Slash:
                if (*it == '#') {
                    if (*oit == '/') return false;
                    if (++oit == origin.end()) ++it;
                    continue;
                } else if (*it == '+') {
                    if (*oit != '/') {
                        ++oit;
                        continue;
                    } else ++it;
                } else {
                    if (*it == '/') ++oit;
                    state = State::None;
                }
                break;
            case State::None:
            default:
                if (*it == '/') {
                    if (*oit != '/') return false;
                    state = State::Slash;
                } else if (*it == '+' || *it == '#') {
                    return false;
                }
                if (*it != *oit) return false;
        }

        ++it, ++oit;
    }

    return it == topic.end() && oit == origin.end();
}


int main(int argc, char *argv[]) {
    Monitor app;
    app.run(argc, argv);

    return 0;
}