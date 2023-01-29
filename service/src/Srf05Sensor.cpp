//
// Created by Ivan Kishchenko on 29.01.2023.
//

#include "Srf05Sensor.h"
#include "core-service/ApplicationEvent.h"
#include "core-service/EventBusService.h"
#include <gpiod.hpp>


void Srf05Sensor::postConstruct(Registry &registry) {
    BaseService::postConstruct(registry);

    _thread = std::thread([this]() {
        info("distance service thread started");
        gpiod::chip chip("gpiochip0");
        logger::info("chip: {}", chip.name());
        logger::info("label: {}", chip.label());
        logger::info("num_lines: {}", chip.num_lines());

        gpiod::line trig = chip.get_line(4);
        trig.request(
                {
                        "incubator",
                        gpiod::line_request::DIRECTION_OUTPUT,
                        0
                },
                0
        );

        gpiod::line echo = chip.get_line(17);
        echo.request(
                {
                        "incubator",
                        gpiod::line_request::DIRECTION_INPUT,
                        0
                },
                0
        );

        trig.set_value(0);
        usleep(2);
        while (!_shutdown) {
            trig.set_value(1);
            usleep(10);
            trig.set_value(0);

            while (!echo.get_value()) { }
            auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
            while (echo.get_value()) { }
            auto end = std::chrono::high_resolution_clock::now().time_since_epoch();
            auto distance = (double) (end - start).count() * 0.0343 / 2000;
            logger::info("Dist: {}", distance);

            //usleep(200000);
            sleep(1);
        }

        info("distance service thread stopped");
    });

    auto &eventManager = registry.getService<EventBusService>();
    eventManager.subscribe<ApplicationCloseEvent>([this](const auto &event) -> bool {
        _shutdown = 0;
        _thread.join();
        return true;
    });
}

const char *Srf05Sensor::name() {
    return "srf05";
}
