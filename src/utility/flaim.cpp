#include "flaim.hpp"
#include <random>
#include <unordered_map>
#include <iostream>
#include <sstream>

std::string generate_token(size_t length)
{
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, chars.size() - 1);

    std::stringstream token;
    for (size_t i = 0; i < length; ++i)
    {
        token << chars[dist(gen)];
    }
    return token.str();
}