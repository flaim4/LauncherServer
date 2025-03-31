#include "login_handler.hpp"
#include "../utility/flaim.hpp"

#include "../launcher_server.hpp"
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;

void handle_login(http::request<http::string_body> &req, http::response<http::string_body> &res)
{
    try
    {
        boost::json::value json_val = boost::json::parse(req.body());
        boost::json::object json_obj = json_val.as_object();

        if (!json_obj.contains("username") || !json_obj.contains("password"))
        {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"error": "Missing username or password"})";
            res.prepare_payload();
            return;
        }

        std::string username = boost::json::value_to<std::string>(json_obj["username"]);
        std::string password = boost::json::value_to<std::string>(json_obj["password"]);

        for (const User &user : users)
        {
            if (user.username == username && user.password == password)
            {
                std::string token = generate_token();

                while (active_sessions.find(token) != active_sessions.end())
                {
                    token = generate_token();
                }

                active_sessions[token] = username;

                std::cout << "User logged in: " << username << " with token: " << token << std::endl;

                boost::json::object response_obj;
                response_obj["token"] = token;

                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = boost::json::serialize(response_obj);
                res.prepare_payload();
                return;
            }
        }

        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Login or password is incorrect"})";
        res.prepare_payload();
    }
    catch (std::exception &e)
    {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Invalid JSON format"})";
        res.prepare_payload();
    }
}