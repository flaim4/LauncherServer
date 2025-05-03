#include <random>
#include "handlers/login_handler.hpp"
#include "handlers/register_handler.hpp"
#include "handlers/logout_handler.hpp"
#include "handlers/manifest.hpp"
#include <unordered_map>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
//#include "handlers/handle_file_download.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include "launcher_server.hpp"
#include <thread>
#include <memory>
#include "utility/router.h"

Router::router _router;

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
    _router.handle_request(req_, res_);

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
        tcp::acceptor acceptor(io_context, {tcp::v4(), 6002});

        async_accept(acceptor, io_context);
        io_context.run();
    } catch (const std::exception &e) {
        std::cerr << "Error in main thread: " << e.what() << std::endl;
    }
}

int main() {
    _router.add_route("/register", http::verb::post, handle_registrer);
    _router.add_route("/login", http::verb::post, handle_login);
    _router.add_route("/logout", http::verb::post, handle_logout);
    _router.add_route("/manifest/v1", http::verb::get, handle_manifest);



    try {
        std::thread t(main_thread);
        t.join();
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
