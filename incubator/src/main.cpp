//
// Created by Kishchenko Ivan on 27/11/2022.
//

#include <Application.h>
#include <Registry.h>
#include <boost/asio/serial_port.hpp>

#include <list>
#include <iostream>

LOG_COMPONENT_SETUP(esp8266, esp8266_logger)

enum Esp01ConnState {
    STATUS_UNKNOWN,
    STATUS_CHECK_VERSION,
    STATUS_WIFI_GET_STATUS,
    STATUS_WIFI_SET_MODE,
    STATUS_WIFI_WAIT_CONNECTION,
    STATUS_MQTT_WAIT_SET_USER_CFG,
    STATUS_MQTT_WAIT_CONNECTION,
    STATUS_MQTT_WAIT_SUB,
};

class SerialPortService : public BaseServiceShared<SerialPortService> {
    const char *MQTT_EVENT_SUBRECV = "+MQTTSUBRECV";
    const char *MQTT_EVENT_WIFI_DISCONNECTED = "WIFI DISCONNECTED";

    std::shared_ptr<boost::asio::serial_port> _serial;

    std::array<char, 1024> _raw{};

    std::string _response;

    struct Command {
        std::string cmd;
        std::function<void(std::string_view data)> callback;
    };

    std::queue<Command> _cmd;

    Esp01ConnState _state{STATUS_UNKNOWN};

    std::string wifiSSID = "alibaba-guest";
    std::string wifiPASS = "";

    std::string clientId = "a24TdEOcf2z.mqttx-client|securemode=2\\,signmethod=hmacsha256\\,timestamp=1698901303093|";
    std::string username = "mqttx-client&a24TdEOcf2z";
    std::string password = "9701be0910906d016c246f3763d1ec7d2ddd96c9bbd5633e5ad10067540f1bd5";
    std::string uri = "a24TdEOcf2z.iot-as-mqtt.ap-southeast-1.aliyuncs.com";

private:
    bool handleEvent(std::string_view reply) {
        if (MQTT_EVENT_SUBRECV == reply.substr(0, strlen(MQTT_EVENT_SUBRECV))) {
            handleMqttEventSubRecv(reply);
            return true;
        } else if (reply.find(MQTT_EVENT_WIFI_DISCONNECTED) != std::string::npos) {
            writeSome("AT+GMR\r\n");
            return true;
        }

        return false;
    }

    void handleMqttEventSubRecv(std::string_view reply) {
    }

    void handleSuccess() {
        switch (_state) {
            case STATUS_UNKNOWN:
            case STATUS_CHECK_VERSION:
                _state = STATUS_WIFI_SET_MODE;
            case STATUS_WIFI_SET_MODE:
                writeSome(fmt::format(R"(AT+CWJAP="{}","{}")" "\r\n", wifiSSID, wifiPASS));
                _state = STATUS_WIFI_WAIT_CONNECTION;
                break;
            case STATUS_WIFI_WAIT_CONNECTION:
                writeSome(fmt::format(
                                  R"(AT+MQTTUSERCFG=0,mqtt,"{}","{}","{}",,,"")",
                                  clientId,
                                  username,
                                  password
                          )
                );
                _state = STATUS_MQTT_WAIT_SET_USER_CFG;
                break;
            case  STATUS_MQTT_WAIT_SET_USER_CFG:
                writeSome(fmt::format(
                                  "AT+MQTTCONN=0,\"{}\",1883,1",
                                  uri
                          )
                );
                _state = STATUS_MQTT_WAIT_CONNECTION;
                break;
            case STATUS_MQTT_WAIT_CONNECTION:
                break;
        }
    }

    void handleError() {
        _state = STATUS_CHECK_VERSION;
        writeSome("AT+GMR\r\n");
    }

    bool parceMessageReport(std::string_view msg) {
        esp8266::log::info("resp> {}", msg);
        if (msg == "ready") {
            writeSome("AT+GMR\r\n");
        } else if (msg == "busy p…") {
            _state = STATUS_UNKNOWN;
        } else if (msg == "OK") {
            handleSuccess();
        } else if (msg == "ERROR") {
            handleError();
        } else {
            handleEvent(msg);
        }
    }

    void asyncRead() {
        _serial->async_read_some(
                boost::asio::buffer(_raw.data(), _raw.size()),
                [this](const boost::system::error_code &ec, size_t bytes) {
                    for (unsigned int i = 0; i < bytes; ++i) {
                        char c = _raw[i];
                        if (c == '\n') {
                            while (_response.back() == '\r') {
                                _response.pop_back();
                            }
                            parceMessageReport(_response);
                            _response.clear();
                        } else {
                            _response += c;
                        }
                    }

                    asyncRead();
                });
    }

    std::size_t writeSome(std::string_view buf) {
        esp8266::log::info(buf.data());
        boost::system::error_code ec;
        return _serial->write_some(boost::asio::buffer(buf), ec);
    }

public:
    const char *name() override {
        return "serial";
    }

    void postConstruct(Registry &registry) override {
        _serial = std::make_shared<boost::asio::serial_port>(registry.getIoService());
        _serial->open("/dev/cu.usbserial-130");

        _serial->set_option(boost::asio::serial_port_base::baud_rate(115200));
        _serial->set_option(boost::asio::serial_port_base::character_size(8));
        _serial->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        _serial->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        _serial->set_option(
                boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

        asyncRead();
    }
};

class Esp8266App : public Application {
public:
    const char *name() override {
        return "esp8266";
    }

protected:
    void setup(Registry &registry) override {
        registry.createService<SerialPortService>();
    }
};

typedef uint8_t bdaddr[8];

struct Barcode {
    char name[64]{};
    bdaddr bdAddr{};
};

namespace detail {
    void set(char *dst, std::string_view src, size_t size) {
        std::copy_n(src.begin(), std::min(size, src.size()), dst);
        if (size > src.size()) {
            std::fill_n(dst + src.size(), size - src.size(), '0');
        }
    }

    template<typename T, std::enable_if_t<std::is_trivially_copyable<T>::value, bool> = true>
    void set(T &dst, const T &src, size_t size) {
        memcpy(&dst, &src, size);
    }
}

template<typename T>
inline void set_prop(T &dst, const std::string_view &src) {
    detail::set(dst, src, sizeof(T));
}

template<typename T>
inline void set_prop(T &dst, const T &src) {
    detail::set(dst, src, sizeof(T));
}

int main(int argc, char *argv[]) {
    Esp8266App app;
    app.run(argc, argv);
//    Barcode barcode{};
//
//    set_prop(barcode.name, "hello");
//    set_prop(barcode.bdAddr, {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07});

    return 0;
}