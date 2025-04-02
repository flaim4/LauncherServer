#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <fstream>
#include "handle_file_download.hpp"
#include <boost/json.hpp>
#include "../launcher_server.hpp"

namespace beast = boost::beast;
namespace http = beast::http;

void handle_file_download(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    boost::json::value json_val;
    try {
        json_val = boost::json::parse(req.body());
    } catch (std::exception &e) {
        res.result(http::status::bad_request);
        res.body() = R"({"error": "Invalid JSON format"})";
        res.set(http::field::content_type, "application/json");
        res.prepare_payload();
        return;
    }

    boost::json::object json_obj = json_val.as_object();

    if (!json_obj.contains("token")) {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Missing token"})";
        res.prepare_payload();
        return;
    }

    std::string token = boost::json::value_to<std::string>(json_obj["token"]);

    if (active_sessions.find(token) != active_sessions.end()) {
        std::string file_path = "C:/Users/flaim/Desktop/LauncherServer/AdditionalHash_module.jar";
        std::ifstream file(file_path, std::ios::binary);

        if (!file) {
            res.result(http::status::not_found);
            res.body() = "File not found";
            res.set(http::field::content_type, "text/plain");
            return;
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();

        res.result(http::status::ok);
        res.set(http::field::content_type, "application/octet-stream");
        res.set(http::field::content_disposition, "attachment; filename=\"test.txt\"");
        res.body() = buffer.str();
    } else {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "Invalid token"})";
        res.prepare_payload();
    }
}
