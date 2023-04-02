//
// Created by Ivan Kishchenko on 02/04/2023.
//

#pragma once

#include "http/HttpRequestHandler.h"
#include "db/Database.h"
#include "Service.h"

class RestRegistryController : public HttpRequestHandler {
    db::Database& _database;
private:
    void handleGet(const HttpRequest &request, const HttpParams& params, HttpResponse &response);
    void handlePost(const HttpRequest &request, HttpResponse &response);
    void handleDelete(const HttpRequest &request, HttpResponse &response);
public:
    RestRegistryController(db::Database& database) : _database(database) {}
    void handle(const HttpRequest &request, HttpResponse &response) override;
};
