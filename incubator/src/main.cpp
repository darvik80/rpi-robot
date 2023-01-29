//
// Created by Kishchenko Ivan on 27/11/2022.
//
#include <gpiod.hpp>

#include "logging/Logging.h"

#ifndef    CONSUMER
#define    CONSUMER    "Consumer"
#endif

#include <pthread.h>

#include "gpiod/pwm.h"

void * threadFunc(void * arg) {

    int* stop = (int*)(arg);
//    int policy;
//    struct sched_param param{};
//
//    pthread_getschedparam(pthread_self(), &policy, &param);
//    param.sched_priority = sched_get_priority_max(policy);
//    pthread_setschedparam(pthread_self(), policy, &param);
//
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
    while (!(*stop)) {
        trig.set_value(1);
        usleep(10);
        trig.set_value(0);

        while (!echo.get_value()) { }
        auto start = std::chrono::high_resolution_clock::now().time_since_epoch();
        while (echo.get_value()) { }
        auto end = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto distance = (double) (end - start).count() * 0.0343 / 2000;
        logger::info("Dist: {}", distance);

        sleep(1);
    }

    return nullptr;
}

#include <thread>

#define PWMB1   16 //   20  28  26
#define PWMB2   20 //   21  29  27
#define PWM2    21 //   26  25  25

int main(int argc, char *argv[]) {
    std::thread thread;
    logger::LoggingProperties logProps;
    logProps.level = "info";
    logger::setup(logProps);
    volatile int stop = 0;
    pthread_t threadId;
    int err = pthread_create(&threadId, nullptr, &threadFunc, (void*)&stop);


    err = pthread_join(threadId, nullptr);

//    gpiod::chip chip("gpiochip0");
//    logger::info("chip: {}", chip.name());
//    logger::info("label: {}", chip.label());
//    logger::info("num_lines: {}", chip.num_lines());
//
//    auto pwma1 = chip.get_line(PWMB1);
//    pwma1.request({"incubator", gpiod::line_request::DIRECTION_OUTPUT,0 },0);
//    auto pwma2 = chip.get_line(PWMB2);
//    pwma2.request({"incubator", gpiod::line_request::DIRECTION_OUTPUT,0 },0);
//    auto pwma = chip.get_line(PWM2);
//    pwma.request({"incubator", gpiod::line_request::DIRECTION_OUTPUT,0 },0);
//
//    gpiod::extension::soft_pwm pwm(chip.get_line(PWM2), 0, 1024);
//
//    pwma1.set_value(1);
//    pwma2.set_value(0);
//
//    pwm.write(1024);
//    sleep(10);
//    pwm.write(512);
//    sleep(10);
//    pwm.write(0);
    return 0;
}
