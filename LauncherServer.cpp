#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    res.version(req.version());
    res.result(http::status::ok);
    res.set(http::field::server, "C++ HTTP Server");
    res.set(http::field::content_type, "text/html");
    res.body() = "<h1>Hello from Boost.Beast!</h1>";
    res.prepare_payload();
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});

        while (true) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            http::response<http::string_body> res;
            handle_request(req, res);

            http::write(socket, res);
            socket.shutdown(tcp::socket::shutdown_send);
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}