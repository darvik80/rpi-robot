//
// Created by Ivan Kishchenko on 26.04.2021.
//

#include "HttpRequestHandlerManager.h"
#include<boost/tokenizer.hpp>

void HttpRequestHandlerManager::process(const HttpRequest& req, HttpResponse& resp) {
    std::string target(req.target().data(), req.target().length());
    boost::char_separator<char> sep{"?"};
    boost::tokenizer tok(target, boost::char_separator<char>{"?"});
    std::string path = *tok.begin();
    while (*path.rbegin() == '/') {
        path.pop_back();
    }
    if (path.empty()) {
        path = "/";
    }

   if (auto it = _handlers.find(Request{req.method(), path}); it != _handlers.end()) {
       it->second(req, resp);
   } else {
       if (it = _handlers.find(Request{req.method(), "/**"}); it != _handlers.end()) {
           it->second(req, resp);
           return;
       }

       throw std::invalid_argument(req.target().data());
   }
}