#pragma once
#include <string>

class AuthService
{
public:
    static bool check(const std::string &login, const std::string &password, std::size_t &token);
};
