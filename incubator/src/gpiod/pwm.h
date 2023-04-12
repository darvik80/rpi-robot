//
// Created by Ivan Kishchenko on 15.01.2023.
//


#pragma once

#include <gpiod.hpp>
#include <pthread.h>

namespace gpiod::extension {
    class soft_pwm {
        volatile int _range{0};
        volatile int _value{0};
        line _line;
        pthread_t _thread{0};

    private:
        static void *thread(void *args);

        [[noreturn]] void process();

    public:
        explicit soft_pwm(line line, int initialValue, int pwmRange);

        void setup(line &line, int initialValue, int pwmRange);

        void write(int value);

        void shutdown();

        ~soft_pwm() {
            shutdown();
        }
    };

}
