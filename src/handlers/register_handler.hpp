#ifndef REGISTER_HANDLER_HPP
#define REGISTER_HANDLER_HPP

#include <boost/beast/http.hpp>

void handle_registrer(const http::request<http::string_body>& req, http::response<http::string_body>& res);

#endif