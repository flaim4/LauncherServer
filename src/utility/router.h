#ifndef DATABSE_HELPER
#define DATABSE_HELPER

#include <functional>
#include <unordered_map>
#include <string>
#include <utility>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

namespace Router {
    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
    
    using handler_type = std::function<void(const http::request<http::string_body>&, http::response<http::string_body>&)>;
    class router {
        public:
            router(const router&) = delete;
            router& operator=(const router&) = delete;
            router(router&&) = delete;
            router& operator=(router&&) = delete;
        
            router() = default;
        
            void add_route(const std::string& path, http::verb method, handler_type handler) {
                routes_[{path, method}] = handler;
            }
        
            void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
                auto key = std::make_pair(std::string(req.target()), req.method());
                auto it = routes_.find(key);
                if (it != routes_.end()) {
                    it->second(req, res);
                } else {
                    res.result(http::status::not_found);
                    res.body() = "Not Found";
                    res.set(http::field::content_type, "text/plain");
                }
            }
        
        private:
            std::unordered_map<std::pair<std::string, http::verb>, handler_type, pair_hash> routes_;
        };
}


#endif