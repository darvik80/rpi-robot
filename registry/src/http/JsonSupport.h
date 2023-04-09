//
// Created by Ivan Kishchenko on 07/04/2023.
//

#pragma once

#include <nlohmann/json.hpp>

template<typename T>
inline void to_json(nlohmann::json &nlohmann_json_j, const Page<T> &page) {
    nlohmann_json_j["data"] = page.data;
    nlohmann_json_j["total"] = page.total;
}

template<typename T>
inline void to_json(nlohmann::json &nlohmann_json_j, const std::optional<T> &item) {
    if (item) {
        nlohmann_json_j["data"] = *item;
    } else {
        nlohmann_json_j["data"] = {};
    }
}
