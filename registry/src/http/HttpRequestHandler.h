//
// Created by Ivan Kishchenko on 26.04.2021.
//

#pragma once

#include <boost/beast/http.hpp>
#include <boost/url.hpp>
#include <functional>
#include <string>
#include <variant>

#include "HttpRequestParams.h"

namespace http = boost::beast::http;

typedef boost::urls::params_view HttpParams;
typedef http::request<http::string_body> HttpRequest;
typedef http::response<http::string_body> HttpStringResponse;
typedef http::response<http::file_body> HttpFileResponse;
typedef std::variant<HttpStringResponse, HttpFileResponse> HttpResponse;

typedef std::function<void(const HttpRequest &, HttpResponse &)> HttpRequestHandlerFn;

class HttpRequestHandler {
public:
    typedef std::shared_ptr<HttpRequestHandler> Ptr;
public:
    virtual void handle(const HttpRequest &, HttpResponse &) = 0;

protected:
    void applyHeaders(HttpStringResponse &resp) {
        resp.set(http::field::access_control_allow_origin, "*");
        resp.set(http::field::access_control_allow_headers, "Content-Type");
        resp.set(http::field::access_control_allow_methods, "GET, POST, DELETE, PUT, PATCH, OPTIONS");
        resp.set(http::field::content_type, "application/json");
    }
};

class HttpRestController : public HttpRequestHandler {
public:
    void handle(const HttpRequest &request, HttpResponse &response) override {
        auto result = boost::urls::parse_origin_form(request.target());
        HttpRequestParams params(result.value().params());

        auto &res = response.emplace<HttpStringResponse>();
        res.set(http::field::content_type, "application/json");
        applyHeaders(res);

        if (params.getFilter().contains("id")) {
            if (request.method() == http::verb::get) {
                handleGetOne(request, params, res);
            } else if (request.method() == http::verb::post) {
                handlePostOne(request, params, res);
            } else if (request.method() == http::verb::put) {
                handlePutOne(request, params, res);
            } else if (request.method() == http::verb::delete_) {
                handleDeleteOne(request, params, res);
            } else {
                defaultHandler(request, params, res);
            }
        } else {
            if (request.method() == http::verb::get) {
                if (params.getFilter().contains("id")) {
                    handleGetOne(request, params, res);
                } else {
                    handleGet(request, params, res);
                }
            } else if (request.method() == http::verb::post) {
                handlePost(request, params, res);
            } else if (request.method() == http::verb::put) {
                handlePut(request, params, res);
            } else if (request.method() == http::verb::delete_) {
                handleDelete(request, params, res);
            } else {
                defaultHandler(request, params, res);
            }
        }
    }

protected:
    virtual void handleGetOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(req, params, resp);
    }

    virtual void handlePostOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(req, params, resp);
    }

    virtual void handlePutOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(req, params, resp);
    }

    virtual void handleDeleteOne(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(req, params, resp);
    }

    virtual void handleGet(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(req, params, resp);
    }

    virtual void handlePost(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(req, params, resp);
    }

    virtual void handlePut(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(req, params, resp);
    }

    virtual void handleDelete(const HttpRequest &request, const HttpRequestParams &params, HttpStringResponse &resp) {
        defaultHandler(request, params, resp);
    }

    virtual void defaultHandler(const HttpRequest &req, const HttpRequestParams &params, HttpStringResponse &resp) {
        resp.body() = R"({"message": "Handler not implemented"})";
    }
};
