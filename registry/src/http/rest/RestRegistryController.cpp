//
// Created by Ivan Kishchenko on 02/04/2023.
//

#include "RestRegistryController.h"
#include <boost/url.hpp>
#include <logging/Logging.h>
#include "db/RegistryRepository.h"

using namespace boost::urls;

void RestRegistryController::handle(const HttpRequest &request, HttpResponse &response) {
    result<url_view> result = parse_origin_form(request.target());

    if (request.method() == http::verb::get) {
        handleGet(request, result.value().params(), response);
    } else {
        auto &resp = response.emplace<HttpStringResponse>();
        resp.body() = "Hello World!";
    }
}

void RestRegistryController::handleGet(const HttpRequest &request, const HttpParams& params, HttpResponse &response) {
    auto repository = _database.getRepository<RegistryRepository>();
    int size = 10;
    if (const auto it = params.find("size"); it != params.end()) {
        size = std::stoi((*it).value);
    }

    int offset = 0;
    if (auto it = params.find_last("page"); it != params.end()) {
        offset = size * std::stoi((*it).value);
    }
    std::string_view name;
    if (auto it = params.find_last("name"); it != params.end()) {
        name = (*it).value;
    }
    std::list<IotRegistry> registries = repository.findAll(name, offset, size);

    nlohmann::json json;
    json["registries"] = registries;
    json["total"] = repository.findAllCount(name);

    auto& res = response.emplace<HttpStringResponse>();
    res.body() = json.dump();
    res.set(http::field::content_type, "application/json");
}

void RestRegistryController::handlePost(const HttpRequest &request, HttpResponse &response) {

}

void RestRegistryController::handleDelete(const HttpRequest &request, HttpResponse &response) {

}
