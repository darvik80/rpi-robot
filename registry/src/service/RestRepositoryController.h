//
// Created by Ivan Kishchenko on 23/04/2023.
//

#pragma once

#include "db/DatabaseSupport.h"
#include "db/Database.h"
#include "http/HttpRequestHandler.h"
#include "http/JsonSupport.h"

template<typename T, typename R = TRepository <T>>
class RestRepositoryController : public HttpRestController {
public:
    void handleGetOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) override {
        nlohmann::json json = getRepository().findOne(params.getFilter());
        resp.body() = json.dump();
    }

    void handleGet(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) override {
        nlohmann::json json = *getRepository().findAll(params.getFilter(), params.getPage());
        resp.body() = json.dump();
    }

    void handlePostOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) override {
        getRepository().insert(nlohmann::json::parse(req.body()));
        resp.body() = R"({"status": "success"})";
    }

    void handlePutOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) override {
        getRepository().update(nlohmann::json::parse(req.body()));
        resp.body() = R"({"status": "success"})";
    }

    void handleDeleteOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) override {
        getRepository().deleteByFilter(params.getFilter());
        resp.body() = R"({"status": "success"})";
    }

protected:
    virtual R &getRepository() = 0;
};

template<typename T, typename R = TRepository <T>>
class DatabaseRestRepositoryController : public RestRepositoryController<T, R> {
    Database &_database;
protected:
    R &getRepository() override{
        return _database.getRepository<R>();
    }
public:
    explicit DatabaseRestRepositoryController(Database &database) : _database(database) {}
};