//
// Created by Kishchenko Ivan on 27/11/2022.
//

#include "logging/Logging.h"
#include "bitmask_operators.h"
#include <system_error>

namespace iot {
    enum class errc {
        success = 0,
        device_unknown,
        device_auth_failed
    };

    namespace detail {

        class iot_category : public std::error_category {
        public:
            const char *name() const noexcept override {
                return "my app";
            }

            [[nodiscard]] std::string message(int ev) const override {
                switch (static_cast<iot::errc>(ev)) {
                    case iot::errc::success:
                        return "SUCCESS";
                    case iot::errc::device_unknown:
                        return "device unknown";
                    case iot::errc::device_auth_failed:
                        return "device auth failed";
                    default:
                        return "unknown";
                }
            }

            [[nodiscard]] bool equivalent(int code, const std::error_condition &condition) const _NOEXCEPT override {
                return error_category::equivalent(code, condition);
            }
        };
    }


    const std::error_category &category() {
        // The category singleton
        static detail::iot_category instance;
        return instance;
    }

    inline std::error_code make_error_code(iot::errc val) {
        // Create an error_code with the original mpg123 error value
        // and the mpg123 error category.
        return {static_cast<int>(val), category()};
    }

    std::error_condition make_error_condition(iot::errc val) {
        return {static_cast<int>(val), category()};
    }
}

namespace std {
    template<>
    struct is_error_code_enum<iot::errc> : public true_type {
    };

    template<>
    struct is_error_condition_enum<iot::errc> : public true_type {
    };
}
#include <array>
#include <iostream>

const double pi = std::acos(-1);

static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

struct WifiConnected {
    char ssid[32];
};

struct MagicAction {
    uint16_t actionId{0};
};

union UserContext {
    WifiConnected wifiConnected;
    MagicAction magicAction;
};

struct UserMessage {
    uint16_t msgId;
};

int main(int argc, char *argv[]) {
    std::array<uint32_t, 16> data;
    for (size_t idx = 0; idx < data.size(); idx++) {
        double val = std::abs(std::sin(pi*2*idx/data.size())*255);
        data[idx] = Color(0, (uint8_t)val, 0);
        std::cout << val << " ";
    }

    std::cout << std::endl;

    return 0;
}
