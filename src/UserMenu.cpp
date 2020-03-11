//
// Created by gryffyn on 10/03/2020.
//

#include "UserMenu.hpp"

#include <iostream>
#include <sstream>

namespace Color {
enum Code {
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BLUE     = 34,
    FG_DEFAULT  = 39,
    ST_INVIS    = 8,
    ST_DEF      = 0
};
std::string setval(Code code, const std::string& input) {
    std::stringstream ss;
    ss << "\033[" << code << "m" << input << "\033[";
    if (code < 30) { ss << "0m"; }
    else { ss << "39m"; }
    return ss.str();
}
}

void create_user() {
    std::cout << "--------------------------------" << std::endl << "Welcome to the USW Cyber Lab.\n";
    std::cout << "Username: ";
}