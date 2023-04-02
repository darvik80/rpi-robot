//
// Created by Ivan Kishchenko on 26.04.2021.
//

#include "HttpRequestHandlerManager.h"

void HttpRequestHandlerManager::process(const HttpRequest& req, HttpResponse& resp) {
   if (auto it = _handlers.find(Request{req.method(), std::string(req.target().data(), req.target().length())}); it != _handlers.end()) {
       it->second(req, resp);
   } else {
       if (it = _handlers.find(Request{req.method(), "/**"}); it != _handlers.end()) {
           it->second(req, resp);
           return;
       }
       throw std::invalid_argument(req.target().data());
   }
}