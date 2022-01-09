//
// Created by Ivan Kishchenko on 10.07.2021.
//

#include "JoystickService.h"
#include "JoystickLogger.h"

namespace xbox {
    void JoystickService::postConstruct(Registry &registry) {
        BaseService::postConstruct(registry);

        _eventManager = registry.getService<EventManagerService>().shared_from_this();

        _stream = std::make_unique<boost::asio::posix::stream_descriptor>(
                registry.getIoService()
        );
        int fd = open("/dev/input/js0", O_RDONLY);
        if (fd > 0) {
            char name[128];
            if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0) {
                strncpy(name, "Unknown", sizeof(name));
            }
            log::info("joystick name: {}", name);

            _stream->assign(fd);
            _stream->async_read_some(
                    boost::asio::buffer(&_events, sizeof(_events)),
                    [this](const boost::system::error_code error, std::size_t readable) {
                        onRead(error, readable);
                    }
            );
        } else {
            log::warning("failed to open joystick");
        }
    }

    void JoystickService::onRead(const boost::system::error_code &error, std::size_t readable) {
        if (!error) {
            Xbox380Event event = _lastEvent;
            std::size_t size = readable / sizeof(js_event);
            bool changed = false;
            for (int idx = 0; idx < size; idx++) {
                log::info("ev: {}, {}, {}", _events[idx].number, _events[idx].type, _events[idx].value);
                if (_events[idx].number == 0 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setAxisX(axis_left, _events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 3 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setAxisX(axis_right, _events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 1 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setAxisY(axis_left, _events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 4 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setAxisY(axis_right, _events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 6 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setAxisX(axis_mid, _events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 7 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setAxisY(axis_mid, _events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 2 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setLt(((int) _events[idx].value + 32768) >> 8);
                    changed = true;
                } else if (_events[idx].number == 5 && JS_EVENT_AXIS == _events[idx].type) {
                    event.setRt(((int) _events[idx].value + 32768) >> 8);
                    changed = true;
                } else if (_events[idx].number == 4 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setRb(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 5 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setRb(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 0 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setBtnA(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 2 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setBtnX(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 3 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setBtnY(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 1 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setBtnB(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 6 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setBtnBack(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 8 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setBtnXbox(_events[idx].value);
                    changed = true;
                } else if (_events[idx].number == 7 && JS_EVENT_BUTTON == _events[idx].type) {
                    event.setBtnStart(_events[idx].value);
                    changed = true;
                }
            }
            if (changed) {
                _eventManager->raiseEvent(event);
            }

            _lastEvent = event;
            _stream->async_read_some(
                    boost::asio::buffer(&_events, sizeof(_events)),
                    [this](const boost::system::error_code error, std::size_t readable) {
                        onRead(error, readable);
                    }
            );

        } else {
            log::warning("ev failed: {}", error.message());
        }
    }

    void JoystickService::preDestroy(Registry &registry) {
        BaseService::preDestroy(registry);
    }
}
