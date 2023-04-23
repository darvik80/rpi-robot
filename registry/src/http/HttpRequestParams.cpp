//
// Created by Ivan Kishchenko on 23/04/2023.
//

#include "HttpRequestParams.h"

HttpRequestParams::HttpRequestParams(boost::urls::params_view &&params) {
    if (auto it = params.find_last("name"); it != params.end()) {
        _filter.add("name", Filter::Operation::op_eq, (*it).value);
    }
    if (auto it = params.find_last("id"); it != params.end()) {
        _filter.add("id", Filter::Operation::op_eq, (*it).value);
    }
    if (auto it = params.find_last("device_id"); it != params.end()) {
        _filter.add("device_id", Filter::Operation::op_eq, (*it).value);
    }
    if (auto it = params.find_last("registry_id"); it != params.end()) {
        _filter.add("registry_id", Filter::Operation::op_eq, (*it).value);
    }

    if (const auto it = params.find("size"); it != params.end()) {
        _page.size = std::stoi((*it).value);
    }

    if (auto it = params.find_last("page"); it != params.end()) {
        _page.offset = _page.size * std::stoi((*it).value);
    }
}

const Filter &HttpRequestParams::getFilter() const {
    return _filter;
}

const PageRequest &HttpRequestParams::getPage() const {
    return _page;
}
