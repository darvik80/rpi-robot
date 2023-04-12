//
// Created by Ivan Kishchenko on 27.04.2021.
//

#pragma once

#include "HttpRequestHandler.h"

class HttpFileHandler : public HttpRequestHandler {
    std::string _root;
public:
    explicit HttpFileHandler(std::string_view root);

    void handle(const HttpRequest &request, HttpResponse &response) override;
};
