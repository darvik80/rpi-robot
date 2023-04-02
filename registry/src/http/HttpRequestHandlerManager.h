//
// Created by Ivan Kishchenko on 26.04.2021.
//

#pragma once

#include <unordered_map>
#include <boost/beast/http.hpp>
#include "HttpRequestHandler.h"

namespace http = boost::beast::http;

class HttpRequestHandlerManager {
public:
    struct Request {
        http::verb method;
        std::string path;

        // Match both first and last names in case
        // of collisions.
        bool operator==(const Request &req) const {
            return method == req.method && path == req.path;
        }
    };

    class RequestFunction {
    public:

        // Use sum of lengths of first and last names
        // as hash function.
        size_t operator()(const Request &req) const {
            return std::hash<std::string>()(req.path + ":" + to_string(req.method).data());
        }
    };
private:
    std::unordered_map<Request, HttpRequestHandlerFn, RequestFunction> _handlers;
public:
    typedef std::shared_ptr<HttpRequestHandlerManager> Ptr;
public:
    void registerHandler(http::verb method, std::string_view path, const HttpRequestHandlerFn& handler) {
        _handlers.emplace(Request{method, path.data()}, handler);
    }

    void registerHandler(http::verb method, std::string_view path, const HttpRequestHandler::Ptr& handler) {
        _handlers.emplace(Request{method, path.data()}, [handler](const HttpRequest &req, HttpResponse &resp) {
            handler->handle(req, resp);
        });
    }

    virtual void process(const HttpRequest &req, HttpResponse &resp);

};
