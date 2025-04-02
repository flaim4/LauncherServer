#include <random>
#include "handlers/login_handler.hpp"
#include "handlers/register_handler.hpp"
#include "handlers/logout_handler.hpp"
#include <unordered_map>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <../src/handlers/handle_file_download.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include "launcher_server.hpp"
#include <thread>
#include <memory>

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

std::vector<User> users;
std::unordered_map<std::string, std::string> active_sessions;
std::vector<std::thread> threads;


http_session::http_session(tcp::socket socket)
    : socket_(std::move(socket)) {
}

void http_session::start() {
    do_read();
}

void http_session::do_read() {
    http::async_read(socket_, buffer_, req_,
                     [self = shared_from_this()](boost::system::error_code ec, std::size_t) {
                         if (!ec) {
                             self->handle_request();
                         }
                     });
}

void http_session::handle_request() {
    if (req_.target() == "/register" && req_.method() == http::verb::post) {
        handle_registrer(req_, res_);
    } else if (req_.target() == "/login" && req_.method() == http::verb::post) {
        handle_login(req_, res_);
    } else if (req_.target() == "/logout" && req_.method() == http::verb::post) {
        handle_logout(req_, res_);
    } else if (req_.target() == "/download" && req_.method() == http::verb::get) {
        handle_file_download(req_, res_);
    } else {
        res_.result(http::status::not_found);
        res_.body() = "Not Found";
        res_.set(http::field::content_type, "text/plain");
    }

    do_write();
}

void http_session::do_write() {
    http::async_write(socket_, res_, [self = shared_from_this()](boost::system::error_code ec, std::size_t) {
        if (!ec || ec == boost::asio::error::eof) {
            boost::system::error_code ignored_ec;
            self->socket_.shutdown(tcp::socket::shutdown_send, ignored_ec);
        }
    });
}

void async_accept(tcp::acceptor &acceptor, boost::asio::io_context &io_context) {
    acceptor.async_accept(
        [&acceptor, &io_context](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<http_session>(std::move(socket))->start();
            }
            async_accept(acceptor, io_context);
        });
}

void main_thread() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});

        async_accept(acceptor, io_context);
        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << "Error in main thread: " << e.what() << std::endl;
    }
}

int main() {
    try {
        std::thread t(main_thread);
        t.join();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
