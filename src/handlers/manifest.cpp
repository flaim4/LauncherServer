#include "manifest.hpp"
#include "../utility/flaim.hpp"

#include "../launcher_server.hpp"

std::vector<Manifest> manifests = {
    {
        "test",
        "test2",
        Description{"smal", "full"},
        Metadata{"1.16.5", "url", Images{"url"}}
    }
};


void handle_manifest(const http::request<http::string_body>& req, http::response<http::string_body>& res)
{
    try
    {
        if (req.method() == http::verb::get) {
            boost::json::array arr;
            for (const auto& m : manifests) {
                arr.push_back(to_json(m));
            }

            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");
            res.body() = boost::json::serialize(arr);
        } else if (req.method() == http::verb::post) {
            boost::json::value json_val = boost::json::parse(req.body());
            boost::json::object json_obj = json_val.as_object();

            if (!json_obj.contains("method")) {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"error": "Missing method"})";
                res.prepare_payload();
                return;
            }

            std::string method = boost::json::value_to<std::string>(json_obj["method"]);

            if (method == "add") {
                if (!json_obj.contains("manifest")) {
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = R"({"error": "Missing manifest"})";
                    res.prepare_payload();
                    return;
                }
                manifests.push_back(from_json_manifest(json_obj["manifest"]));
            }
            else if (method == "remove") {
                if (!json_obj.contains("id")) {
                    res.result(http::status::bad_request);
                    res.set(http::field::content_type, "application/json");
                    res.body() = R"({"error": "Missing id"})";
                    res.prepare_payload();
                    return;
                }
                std::string id = boost::json::value_to<std::string>(json_obj.at("id"));
                manifests.erase(
                    std::remove_if(manifests.begin(), manifests.end(),
                        [&](const Manifest& m) {
                            return m.id == id;
                        }),
                    manifests.end()
                );
            }
            else {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({"error": "Invalid method"})";
            }
        }
    }
    catch (std::exception &e)
    {
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "application/json");
        res.body() = R"({"error": "501"})";
    }
    res.prepare_payload();
}

Description from_json_description(const boost::json::value& v) {
    const auto& obj = v.as_object();
    return {
        boost::json::value_to<std::string>(obj.at("smal")),
        boost::json::value_to<std::string>(obj.at("ful"))
    };
}

Images from_json_images(const boost::json::value& v) {
    const auto& obj = v.as_object();
    return {
        boost::json::value_to<std::string>(obj.at("icon"))
    };
}

Metadata from_json_metadata(const boost::json::value& v) {
    const auto& obj = v.as_object();
    return {
        boost::json::value_to<std::string>(obj.at("version")),
        boost::json::value_to<std::string>(obj.at("manifest")),
        from_json_images(obj.at("images"))
    };
}

Manifest from_json_manifest(const boost::json::value& v) {
    const auto& obj = v.as_object();
    return {
        boost::json::value_to<std::string>(obj.at("name")),
        boost::json::value_to<std::string>(obj.at("id")),
        from_json_description(obj.at("description")),
        from_json_metadata(obj.at("metadata"))
    };
}

boost::json::value to_json(const Description& d) {
    return {
        {"smal", d.smal},
        {"ful", d.ful}
    };
}

boost::json::value to_json(const Images& i) {
    return {
        {"icon", i.icon}
    };
}

boost::json::value to_json(const Metadata& m) {
    return {
        {"version", m.version},
        {"manifest", m.manifest},
        {"images", to_json(m.images)}
    };
}

boost::json::value to_json(const Manifest& m) {
    return {
        {"name", m.name},
        {"id", m.id},
        {"description", to_json(m.description)},
        {"metadata", to_json(m.metadata)}
    };
}