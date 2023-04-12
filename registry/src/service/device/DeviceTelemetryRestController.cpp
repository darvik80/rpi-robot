//
// Created by Ivan Kishchenko on 11/04/2023.
//

#include "DeviceTelemetryRestController.h"

#include <boost/url.hpp>
#include "service/device/DeviceTelemetryRepository.h"
#include "http/JsonSupport.h"

using namespace boost::urls;

void DeviceTelemetryRestController::handle(const HttpRequest &request, HttpResponse &response) {
    result<url_view> result = parse_origin_form(request.target());

    if (request.method() == http::verb::get) {
        handleGet(request, result.value().params(), response);
    } else if (request.method() == http::verb::delete_) {
        handleDelete(request, result.value().params(), response);
    } else {
        auto &resp = response.emplace<HttpStringResponse>();
        applyHeaders(resp);
    }
}

void
DeviceTelemetryRestController::handleGet(const HttpRequest &request, const HttpParams &params, HttpResponse &response) {
    auto repository = _database.getRepository<DeviceTelemetryRepository>();

    nlohmann::json json;
    if (params.contains("id")) {
        json = repository.findById(std::stoi((*params.find_last("id")).value));
    } else {
        PageRequest page{.offset = 0, .size = 10, .hasCount=true};
        if (const auto it = params.find("size"); it != params.end()) {
            page.size = std::stoi((*it).value);
        }

        if (auto it = params.find_last("page"); it != params.end()) {
            page.offset = page.size * std::stoi((*it).value);
        }

        Filter filter;
        json = *repository.findAll(filter, page);
    }

    auto &res = response.emplace<HttpStringResponse>();
    res.body() = json.dump();
    applyHeaders(res);
}

void DeviceTelemetryRestController::handleDelete(const HttpRequest &request, const HttpParams &params,
                                                 HttpResponse &response) {
    auto repository = _database.getRepository<DeviceTelemetryRepository>();

    if (params.contains("id")) {
        repository.remove(std::stoi((*params.find_last("id")).value));
    }

    auto &res = response.emplace<HttpStringResponse>();
    res.body() = R"({"status": "success"})";
    applyHeaders(res);
}