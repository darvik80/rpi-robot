//
// Created by Kishchenko Ivan on 27/11/2022.
//

#include "logging/Logging.h"
#include "bitmask_operators.hpp"
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

#include <iostream>

int main(int argc, char *argv[]) {
    ERenderPass primary = ERenderPass::Geometry | ERenderPass::Lighting;

    if ((primary & ERenderPass::Geometry) == ERenderPass::Geometry) {
        std::cout << "ERenderPass::Geometry" << std::endl;
    }

    auto ec1 = iot::make_error_code(iot::errc::device_unknown);
    logger::info("val: {}", ec1.value());
    logger::info("msg: {}", ec1.message());

    auto ec2 = iot::make_error_code(iot::errc::device_auth_failed);

    logger::info("val: {}", ec2.value());
    logger::info("msg: {}", ec2.message());

    if (ec1 == ec2) {
        logger::info("same errors");
    }

    return 0;
}
