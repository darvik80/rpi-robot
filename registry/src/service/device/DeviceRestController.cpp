//
// Created by Ivan Kishchenko on 02/04/2023.
//

#include "DeviceRestController.h"
#include <boost/url.hpp>
#include "service/device/DeviceRepository.h"
#include "http/JsonSupport.h"

using namespace boost::urls;

void DeviceRestController::handle(const HttpRequest &request, HttpResponse &response) {
    result<url_view> result = parse_origin_form(request.target());

    if (request.method() == http::verb::get) {
        handleGet(request, result.value().params(), response);
    } else {
        auto &resp = response.emplace<HttpStringResponse>();
        applyHeaders(resp);
    }
}

void DeviceRestController::handleGet(const HttpRequest &request, const HttpParams &params, HttpResponse &response) {
    auto repository = _database.getRepository<DeviceRepository>();

    auto &res = response.emplace<HttpStringResponse>();
    res.set(http::field::content_type, "application/json");
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
        if (auto it = params.find_last("name"); it != params.end()) {
            filter.add("name", Filter::Operation::op_eq, (*it).value);
        }
        if (auto it = params.find_last("registry_id"); it != params.end()) {
            filter.add("registry_id", Filter::Operation::op_eq, (*it).value);
        }
        json = *repository.findAll(filter, page);
    }

    res.body() = json.dump();
}

void DeviceRestController::handlePost(const HttpRequest &request, HttpResponse &response) {

}

void DeviceRestController::handleDelete(const HttpRequest &request, HttpResponse &response) {

}
