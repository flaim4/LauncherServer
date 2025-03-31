#include "login_handler.hpp"
#include "../utility/flaim.hpp"


#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <iostream>
#include "../launcher_server.hpp"

void handle_registrer(http::request<http::string_body> &req, http::response<http::string_body> &res)
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
            if (user.username == username)
            {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"error": "Username already taken"})";
                res.prepare_payload();
                return;
            }
        }

        users.push_back({username, password});

        std::cout << "New user registered: " << username << std::endl;

        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"message": "You have successfully registered."})";
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