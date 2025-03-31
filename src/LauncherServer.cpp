#include <random>
#include "handlers/login_handler.hpp"
#include "handlers/register_handler.hpp"
#include "handlers/logout_handler.hpp"
#include <unordered_map>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include "launcher_server.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

std::vector<User> users;
std::unordered_map<std::string, std::string> active_sessions;

int main()
{
    try
    {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, {tcp::v4(), 8080});

        while (true)
        {
            tcp::socket socket(io_context);

            acceptor.accept(socket);

            beast::flat_buffer buffer;

            http::request<http::string_body> req;

            http::read(socket, buffer, req);

            http::response<http::string_body> res;

            if (req.target() == "/register" && req.method() == http::verb::post)
            {
                handle_registrer(req, res);
            }
            else if (req.target() == "/login" && req.method() == http::verb::post)
            {
                handle_login(req, res);
            }
            else if (req.target() == "/logout" && req.method() == http::verb::post)
            {
                handle_logout(req, res);
            }
            else
            {
                res.result(http::status::not_found);
                res.body() = "Not Found";
                res.set(http::field::content_type, "text/plain");
            }

            http::write(socket, res);

            socket.shutdown(tcp::socket::shutdown_send);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}