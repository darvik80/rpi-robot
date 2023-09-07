//
// Created by Kishchenko Ivan on 27/11/2022.
//

#include "logging/Logging.h"
#include "bitmask_operators.hpp"
#include <system_error>

typedef uint_least8_t MsgId;

struct Message {
    Message() = default;
    [[nodiscard]] virtual MsgId getMsgId() const = 0;
};

template<MsgId id>
struct TMessage : Message {
public:
    enum {
        ID = id
    };

    [[nodiscard]] MsgId getMsgId() const override {
        return ID;
    }
};

template<MsgId id, typename T>
struct TContainer : TMessage<id> {
    T msg;
};

struct ApplicationMessage : TMessage<0xff> {
    std::string message;
};

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

#include <iostream>
#include <iomanip>

#include <etl/string.h>

struct SimpleStruct {
   char ssid[32];
};

struct EtlStruct {
    etl::string<32> ssid;
};

int main(int argc, char *argv[]) {

    std::cout << "is trivial SimpleStruct: " <<etl::is_trivially_copyable_v<SimpleStruct> << std::endl;

    std::cout << "is trivial EtlStruct: " <<std::is_trivially_copyable_v<EtlStruct> << std::endl;

    uint8_t p[] = { 0x01, 0x0F, 0x03, 0x04, 0x05, 0x06 };
    std::stringstream res;
    res << std::uppercase << std::setfill('0') << std::hex;
    res << std::setw(2) << (int)p[0] << ":";
    res << std::setw(2) << (int)p[1] << ":";
    res << std::setw(2) << (int)p[2] << ":";
    res << std::setw(2) << (int)p[3] << ":";
    res << std::setw(2) << (int)p[4] << ":";
    res << std::setw(2) << (int)p[5];

    std::cout << res.str();
    return 0;
}
