#pragma once
#ifndef MANIFEST
#define MANIFEST

#include <string>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;

struct Description {
    std::string smal;
    std::string ful;
};

struct Images {
    std::string icon;
};

struct Metadata {
    std::string version;
    std::string manifest;
    Images images;
};

struct Manifest {
    std::string name;
    std::string id;
    Description description;
    Metadata metadata;
};

boost::json::value to_json(const Description&);
boost::json::value to_json(const Images&);
boost::json::value to_json(const Metadata&);
boost::json::value to_json(const Manifest&);

Description from_json_description(const boost::json::value&);
Images from_json_images(const boost::json::value&);
Metadata from_json_metadata(const boost::json::value&);
Manifest from_json_manifest(const boost::json::value&);

void handle_manifest(const http::request<http::string_body>& req, http::response<http::string_body>& res);

#endif