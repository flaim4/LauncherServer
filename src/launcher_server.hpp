#ifndef LAUNCHER_SERVER_H
#define LAUNCHER_SERVER_H

#include <vector>
#include <unordered_map>
#include <string>

struct User {
    std::string username;
    std::string password;
};

extern std::vector<User> users;
extern std::unordered_map<std::string, std::string> active_sessions;

#endif
