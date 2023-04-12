//
// Created by Ivan Kishchenko on 15.01.2023.
//

#include <sys/time.h>
#include "pwm.h"

namespace gpiod::extension {
    void delayMicrosecondsHard(unsigned int howLong) {
        struct timeval tNow{}, tLong{}, tEnd{};

        gettimeofday(&tNow, nullptr);
        tLong.tv_sec = howLong / 1000000;
        tLong.tv_usec = howLong % 1000000;
        timeradd(&tNow, &tLong, &tEnd);

        while (timercmp(&tNow, &tEnd, <))
            gettimeofday(&tNow, nullptr);
    }

    void delayMicroseconds(unsigned int howLong) {
        struct timespec sleeper{};
        unsigned int uSecs = howLong % 1000000;
        unsigned int wSecs = howLong / 1000000;

        if (howLong == 0)
            return;
        else if (howLong < 100)
            delayMicrosecondsHard(howLong);
        else {
            sleeper.tv_sec = wSecs;
            sleeper.tv_nsec = (long) (uSecs * 1000L);
            nanosleep(&sleeper, nullptr);
        }
    }

    soft_pwm::soft_pwm(line line, int initialValue, int pwmRange) {
        setup(line, initialValue, pwmRange);
    }

    void *soft_pwm::thread(void *args) {
        struct sched_param param{};
        param.sched_priority = sched_get_priority_max(SCHED_RR);
        pthread_setschedparam(pthread_self(), SCHED_RR, &param);

        struct sched_param sched{0};

        sched.sched_priority = sched_get_priority_max(SCHED_RR);

        sched_setscheduler(0, SCHED_RR, &sched);

        ((soft_pwm *) args)->process();

        return nullptr;
    }

    void soft_pwm::setup(line &line, int initialValue, int pwmRange) {
        if (!_thread) {
            shutdown();
        }

        _line = line;
        _value = initialValue;
        _range = pwmRange;
        pthread_create(&_thread, nullptr, thread, (void *) this);
    }

    void soft_pwm::write(int value) {
        if (value < 0)
            value = 0;
        else if (value > _range)
            value = _range;
        _value = value;
    }

    void soft_pwm::shutdown() {
        if (!_thread) {
            pthread_cancel(_thread);
            pthread_join(_thread, nullptr);
        }

        _thread = 0;
    }

    [[noreturn]] void soft_pwm::process() {
        for (;;) {
            int mark = _value, space = _range - _value;

            if (mark != 0) {
                _line.set_value(1);
            }
            delayMicroseconds(mark * 100);

            if (space != 0) {
                _line.set_value(0);
            }
            delayMicroseconds(space * 100);
        }
    }

}
