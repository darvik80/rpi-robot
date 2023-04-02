//
// Created by Ivan Kishchenko on 27.04.2021.
//

#include "HttpFileHandler.h"
#include <boost/beast/version.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost;

// Return a reasonable mime type based on the extension of a file.
std::string mimeType(std::string_view path) {
    using beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (std::string_view::npos == pos) {
            return std::string_view{};
        }

        return path.substr(pos);
    }();

    if (algorithm::iequals(ext, ".htm")) return "text/html";
    if (algorithm::iequals(ext, ".html")) return "text/html";
    if (algorithm::iequals(ext, ".php")) return "text/html";
    if (algorithm::iequals(ext, ".css")) return "text/css";
    if (algorithm::iequals(ext, ".txt")) return "text/plain";
    if (algorithm::iequals(ext, ".js")) return "application/javascript";
    if (algorithm::iequals(ext, ".json")) return "application/json";
    if (algorithm::iequals(ext, ".xml")) return "application/xml";
    if (algorithm::iequals(ext, ".swf")) return "application/x-shockwave-flash";
    if (algorithm::iequals(ext, ".flv")) return "video/x-flv";
    if (algorithm::iequals(ext, ".png")) return "image/png";
    if (algorithm::iequals(ext, ".jpe")) return "image/jpeg";
    if (algorithm::iequals(ext, ".jpeg")) return "image/jpeg";
    if (algorithm::iequals(ext, ".jpg")) return "image/jpeg";
    if (algorithm::iequals(ext, ".gif")) return "image/gif";
    if (algorithm::iequals(ext, ".bmp")) return "image/bmp";
    if (algorithm::iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
    if (algorithm::iequals(ext, ".tiff")) return "image/tiff";
    if (algorithm::iequals(ext, ".tif")) return "image/tiff";
    if (algorithm::iequals(ext, ".svg")) return "image/svg+xml";
    if (algorithm::iequals(ext, ".svgz")) return "image/svg+xml";

    return "application/text";
}

HttpFileHandler::HttpFileHandler(std::string_view root) : _root(root) {

}

void HttpFileHandler::handle(const HttpRequest &request, HttpResponse &response) {
    std::string target(request.target().data(), request.target().length());
    if (target.empty() || target[0] != '/' || target.find("..") != std::string::npos) {
        throw std::invalid_argument("file not found");
    }

    if (target == "/") {
        target = "/index.html";
    }

    std::string fullPath = _root + target;

    http::file_body::value_type file;
    beast::error_code ec;
    file.open(fullPath.c_str(), beast::file_mode::read, ec);
    if (ec) {
        throw std::invalid_argument("file not found");
    }

    auto& resp = response.emplace<HttpFileResponse>();
    resp.set(http::field::content_type, mimeType(target.data()));
    resp.body() = std::move(file);
    resp.result(http::status::ok);
}
