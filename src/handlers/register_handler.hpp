#ifndef REGISTER_HANDLER_HPP
#define REGISTER_HANDLER_HPP

#include <boost/beast/http.hpp>

void handle_registrer(boost::beast::http::request<boost::beast::http::string_body>& req,
                      boost::beast::http::response<boost::beast::http::string_body>& res);

#endif