//
// Created by Ivan Kishchenko on 2019-07-13.
//

#include "HttpWorker.h"

#include <utility>
#include <iostream>

using namespace boost;

HttpWorker::HttpWorker(asio::io_service &service, std::string_view address, uint16_t port, const HttpRequestHandlerManager::Ptr &handler)
        : _acceptor(service, {asio::ip::make_address(address), port})
        , _socket(service)
        , _handler(handler)
        , _requestDeadline(service, (std::chrono::steady_clock::time_point::max) ())
        , _serializer(0)
{
}

void HttpWorker::start() {
    accept();
    checkDeadline();
}

void HttpWorker::checkDeadline() {
    // The deadline may have moved, so check it has really passed.
    if (_requestDeadline.expiry() <= std::chrono::steady_clock::now()) {
        _socket.close();

        // Sleep indefinitely until we're given a new deadline.
        _requestDeadline.expires_at(std::chrono::steady_clock::time_point::max());
    }

    _requestDeadline.async_wait([this](beast::error_code) {
        checkDeadline();
    });
}

void HttpWorker::accept() {
    // Clean up any previous connection.
    beast::error_code ec;
    _socket.close(ec);
    _buffer.consume(_buffer.size());

    _acceptor.async_accept(
            _socket,
            [this](beast::error_code ec) {
                if (ec) {
                    accept();
                } else {
                    // Request must be fully processed within 60 seconds.
                    _requestDeadline.expires_after(std::chrono::seconds(60));

                    readRequest();
                }
            });
}


void HttpWorker::readRequest() {
    _parser.emplace();

    http::async_read(
            _socket,
            _buffer,
            *_parser,
            [this](beast::error_code ec, size_t) {
                if (ec) {
                    accept();
                } else {
                    process(_parser->get());
                }
            });
}

void HttpWorker::process(const HttpRequest &req) {
    try {
        _handler->process(req, _response);
        std::visit(*this, _response);
    } catch (std::invalid_argument& ex) {
        sendBadResponse(http::status::not_found, ex.what());
    } catch (std::exception& ex) {
        sendBadResponse(http::status::bad_request, ex.what());
    }
}

void HttpWorker::visit(HttpStringResponse& resp) {
    resp.set(http::field::server, "Beast");
    resp.keep_alive(false);
    resp.prepare_payload();

    auto& serializer = _serializer.emplace<HttpStringSerializer>(resp);
    http::async_write(
            _socket,
            serializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                accept();
            }
    );
}

void HttpWorker::visit(HttpFileResponse& resp) {
    resp.set(http::field::server, "Beast");
    resp.keep_alive(false);
    resp.prepare_payload();

    auto& serializer = _serializer.emplace<HttpFileSerializer>(resp);

    http::async_write(
            _socket,
            serializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                accept();
            }
    );
}

void HttpWorker::sendBadResponse(http::status status, std::string_view error) {
    auto& resp = _response.emplace<HttpStringResponse>();
    resp.result(status);
    resp.keep_alive(false);
    resp.set(http::field::server, "Beast");
    resp.set(http::field::content_type, "text/plain");
    resp.body() = error;
    resp.prepare_payload();

    auto& serializer = _serializer.emplace<HttpStringSerializer>(resp);

    http::async_write(
            _socket,
            serializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                accept();
            }
    );
}

void HttpWorker::shutdown() {
    _acceptor.close();
}

