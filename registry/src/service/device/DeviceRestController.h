//
// Created by Ivan Kishchenko on 02/04/2023.
//

#pragma once

#include "http/HttpRequestHandler.h"
#include "db/Database.h"

class DeviceRestController : public HttpRequestHandler {
    Database &_database;
private:
    void handleGet(const HttpRequest &request, const HttpParams &params, HttpResponse &response);

    void handlePost(const HttpRequest &request, HttpResponse &response);

    void handleDelete(const HttpRequest &request, HttpResponse &response);

public:
    explicit DeviceRestController(Database &database) : _database(database) {}

    void handle(const HttpRequest &request, HttpResponse &response) override;
};