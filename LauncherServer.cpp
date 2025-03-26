#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <boost/asio.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

struct User {
    std::string username;
    std::string password;
};

std::vector<User> users;

void handle_registrer(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    try {
        boost::json::value json_val = boost::json::parse(req.body());
        boost::json::object json_obj = json_val.as_object();

        if (!json_obj.contains("username") || !json_obj.contains("password")) {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Missing username or password"})";
            res.prepare_payload();
            return;
        }

        std::string username = boost::json::value_to<std::string>(json_obj["username"]);
        std::string password = boost::json::value_to<std::string>(json_obj["password"]);

        users.push_back({username, password});

        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"message": "You have successfully registered."})";
        res.prepare_payload();
    } catch(std::exception& e) {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() =  R"({"error": "Invalid JSON format"})";
        res.prepare_payload();
    }
}

void handle_sing_in(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    try {
        
    } catch(std::exception& e) {
        
    }
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
            
            if (req.target() == "/register" && req.method() == http::verb::post) {
                handle_registrer(req, res);
            } else {
                res.result(http::status::not_found);
                res.body() = "Not Found";
                res.set(http::field::content_type, "text/plain");
            }

            http::write(socket, res);
            
            socket.shutdown(tcp::socket::shutdown_send);
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
