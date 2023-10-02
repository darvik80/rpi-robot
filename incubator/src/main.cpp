//
// Created by Kishchenko Ivan on 27/11/2022.
//
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <cjson/cJSON.h>
#include <unistd.h>

struct MqttProperties {
    struct BrokerInfo {
        std::string uri;
        std::optional<std::string> username;
        std::optional<std::string> password;
        std::optional<std::string> caCert;
        std::optional<std::string> clientCert;
        std::optional<std::string> clientKey;
        std::string deviceName;
        std::string productName;
    };

    std::vector<BrokerInfo> brokers;
    int retries{3};
};

void fromJson(cJSON *json, MqttProperties::BrokerInfo &props) {
    cJSON *item = json->child;
    while (item) {
        if (!strcmp(item->string, "uri") && item->type == cJSON_String) {
            props.uri = item->valuestring;
        } else if (!strcmp(item->string, "username") && item->type == cJSON_String) {
            props.username = item->valuestring;
        } else if (!strcmp(item->string, "password") && item->type == cJSON_String) {
            props.password = item->valuestring;
        } else if (!strcmp(item->string, "ca-cert-file") && item->type == cJSON_String) {
            props.caCert = item->valuestring;
        } else if (!strcmp(item->string, "client-cert") && item->type == cJSON_String) {
            props.clientCert = item->valuestring;
        } else if (!strcmp(item->string, "client-key-file") && item->type == cJSON_String) {
            props.clientKey = item->valuestring;
        } else if (!strcmp(item->string, "device-name") && item->type == cJSON_String) {
            props.deviceName = item->valuestring;
        } else if (!strcmp(item->string, "product-name") && item->type == cJSON_String) {
            props.productName = item->valuestring;
        }

        item = item->next;
    }
}

void fromJson(cJSON *json, MqttProperties &props) {
    MqttProperties::BrokerInfo info;
    if (json->type == cJSON_Object) {
        fromJson(json, info);
        props.brokers.push_back(info);
    } else if (json->type == cJSON_Array) {
        cJSON *item = json->child;
        while (item) {
            if (item->type == cJSON_Object) {
                fromJson(item, info);
                props.brokers.push_back(info);
            }
            item = item->next;
        }
    }

}

std::string jsonConfig = R"(
{
  "mqtt": {
        "type": "local",
        "uri": "mqtt://192.168.0.3",
        "username": "mqtt",
        "password": "mqtt",
        "device-name": "magic-lamp",
        "product-name": "darvik-home"
    }
}
)";

class MqttLoadBalancer {
    int _retries;
    int _iterRetries{0};
    std::vector<MqttProperties::BrokerInfo>::iterator _iter;
    std::vector<MqttProperties::BrokerInfo> _brokers;
public:
    explicit MqttLoadBalancer(const MqttProperties &props) : _brokers(props.brokers), _retries(props.retries) {
        _iter = _brokers.end();
    }

    std::tuple<MqttProperties::BrokerInfo &, bool> getNextBroker() {
        if (--_iterRetries <= 0) {
            _iterRetries = _retries;
            if (_iter == _brokers.end()) {
                _iter = _brokers.begin();
            } else if (++_iter == _brokers.end()) {
                _iter = _brokers.begin();
            }

            return {*_iter, true};
        }

        return {*_iter, false};
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

    if (it != topic.end() && *it == '+') {
        ++it;
    }

    return it == topic.end() && oit == origin.end();
}


int main(int argc, char *argv[]) {
    std::cout << compareTopics("/registry/device/user/custom-topic", "/registry/device/user/custom-topic") << std::endl;
    std::cout << compareTopics("/registry/device/+/+", "/registry/device/user/custom-topic") << std::endl;
    std::cout << compareTopics("/user/custom-topic", "/registry/device/user/custom-topic") << std::endl;

    std::string topic = "/registry/device/user/custom-topic";
    MqttProperties props;

    cJSON *json = cJSON_ParseWithLength(jsonConfig.c_str(), jsonConfig.size());
    cJSON *item = json->child;
    while (item) {
        if (0 == strcmp(item->string, "mqtt")) {
            fromJson(item, props);
        }
        item = item->next;
    }

    cJSON_free(json);

    MqttLoadBalancer balancer(props);

    while (true) {
        auto [broker, status] = balancer.getNextBroker();
        if (status) {
            std::cout << "updated server: " << broker.uri << std::endl;
        } else {
            std::cout << "cur server: " << broker.uri << std::endl;
        }
        sleep(2);
    }

    return 0;
}
