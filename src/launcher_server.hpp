#ifndef LAUNCHER_SERVER_H
#define LAUNCHER_SERVER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <thread>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>


using tcp = boost::asio::ip::tcp;

struct User {
    std::string username;
    std::string password;
};

extern std::vector<User> users;
extern std::unordered_map<std::string, std::string> active_sessions;
extern std::vector<std::thread> threads;

class http_session : public std::enable_shared_from_this<http_session> {
public:
    http_session(tcp::socket socket);

    void start();

private:
    tcp::socket socket_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> req_;
    boost::beast::http::response<boost::beast::http::string_body> res_;

    void do_read();

    void handle_request();

    void do_write();
};

#endif
