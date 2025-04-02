#ifndef HANDLE_FILE_DOWNLOAD_H
#define HANDLE_FILE_DOWNLOAD_H

#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

void handle_file_download(const http::request<http::string_body>& req, http::response<http::string_body>& res);

#endif