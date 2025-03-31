#ifndef LOGIN_HANDLER_HPP
#define LOGIN_HANDLER_HPP

#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

void handle_login(http::request<http::string_body> &req, http::response<http::string_body> &res);

#endif
