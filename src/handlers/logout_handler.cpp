#include <iostream>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include "../launcher_server.hpp"
#include "logout_handler.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

void handle_logout(http::request<http::string_body> &req, http::response<http::string_body> &res)
{
    try
    {
        boost::json::value json_val = boost::json::parse(req.body());
        boost::json::object json_obj = json_val.as_object();

        if (!json_obj.contains("token"))
        {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Missing token"})";
            res.prepare_payload();
            return;
        }

        std::string token = boost::json::value_to<std::string>(json_obj["token"]);

        if (active_sessions.find(token) != active_sessions.end())
        {
            active_sessions.erase(active_sessions.find(token));
            std::cout << "User logged out with token: " << token << std::endl;
            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"message": "You have successfully logged out."})";
            res.prepare_payload();
        }
        else
        {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Invalid token"})";
            res.prepare_payload();
        }
    }
    catch (std::exception &e)
    {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Invalid JSON format"})";
        res.prepare_payload();
    }
}
