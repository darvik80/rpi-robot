//
// Created by Ivan Kishchenko on 11/04/2023.
//

#pragma once

#include "http/HttpRequestHandler.h"
#include "db/Database.h"


class DeviceTelemetryRestController : public HttpRequestHandler {
    Database &_database;
private:
    void handleGet(const HttpRequest &request, const HttpParams &params, HttpResponse &response);

public:
    explicit DeviceTelemetryRestController(Database &database) : _database(database) {}

    void handle(const HttpRequest &request, HttpResponse &response) override;

};
