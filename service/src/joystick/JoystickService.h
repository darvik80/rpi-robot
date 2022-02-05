//
// Created by Ivan Kishchenko on 10.07.2021.
//

#pragma once

#ifdef RASPBERRY_ARCH

#include "BaseService.h"
#include <boost/asio/posix/stream_descriptor.hpp>
#include <linux/joystick.h>
#include <event/EventManagerService.h>
#include "JoystickEvent.h"

namespace xbox {

    class JoystickService : public BaseServiceShared<JoystickService> {
        js_event _events[64];
        std::unique_ptr<boost::asio::posix::stream_descriptor> _stream;

        Xbox380Event _lastEvent;
        em::EventManager::Ptr _eventManager;
    public:
        const char *name() override {
            return "joystick";
        }

        void postConstruct(Registry &registry) override;

        void preDestroy(Registry &registry) override;

    private:
        void onRead(const boost::system::error_code &error, std::size_t readable);
    };
}

#endif