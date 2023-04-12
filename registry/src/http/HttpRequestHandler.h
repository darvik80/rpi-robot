//
// Created by Ivan Kishchenko on 26.04.2021.
//

#pragma once

#include <boost/beast/http.hpp>
#include <boost/url.hpp>
#include <functional>
#include <string>
#include <variant>


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
    void applyHeaders(HttpStringResponse& response) {
        response.set(http::field::access_control_allow_origin, "*");
        response.set(http::field::access_control_allow_headers, "Content-Type");
        response.set(http::field::access_control_allow_methods, "GET, POST, DELETE, PUT, PATCH, OPTIONS");
        response.set(http::field::content_type, "application/json");
    }
};
