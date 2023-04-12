#include <utility>

//
// Created by Ivan Kishchenko on 2019-07-13.
//

#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "HttpRequestHandlerManager.h"

namespace ip = boost::asio::ip;         // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace asio = boost::asio;           // from <boost/beast/http.hpp>

typedef http::request_parser<http::string_body> HttpRequestParser;
typedef http::response_serializer<http::string_body> HttpStringSerializer;
typedef http::response_serializer<http::file_body> HttpFileSerializer;
typedef std::variant<HttpStringSerializer, HttpFileSerializer, int> HttpSerializer;

class HttpWorker {
public:
    HttpWorker(HttpWorker const &) = delete;

    HttpWorker &operator=(HttpWorker const &) = delete;

    HttpWorker(asio::io_service &service, std::string_view address, uint16_t port,
               const HttpRequestHandlerManager::Ptr &_handler);

    void start();

    void shutdown();

    void operator()(HttpStringResponse &resp) {
        visit(resp);
    }

    void operator()(HttpFileResponse &resp) {
        visit(resp);
    }

private:
    // The acceptor used to listen for incoming connections.
    tcp::acceptor _acceptor;

    // The socket for the currently connected client.
    tcp::socket _socket;

    // The path to the root of the document directory.
    std::string _docRoot;

    // The buffer for performing reads
    boost::beast::flat_static_buffer<32768> _buffer;

    // The parser for reading the requests
    std::optional<HttpRequestParser> _parser;

    // The timer putting a time limit on requests.
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> _requestDeadline;


    HttpResponse _response;
    HttpSerializer _serializer;

    HttpRequestHandlerManager::Ptr _handler;
private:
    void accept();

    void readRequest();

    void sendBadResponse(http::status status, std::string_view error);

    void checkDeadline();

    void rpc(const HttpRequest &req);

    void process(const HttpRequest &req);

    void visit(HttpStringResponse &resp);

    void visit(HttpFileResponse &resp);
};
