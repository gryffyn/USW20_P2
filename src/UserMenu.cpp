//
// Created by gryffyn on 10/03/2020.
//

#include "UserMenu.hpp"

#include <iostream>
#include <sstream>
#include "ObjStore.hpp"
#include <mariadb++/connection.hpp>
#include "Key.hpp"

namespace Color {
enum Code {
    FG_RED      = 31,
    FG_GREEN    = 32,
    FG_BLUE     = 34,
    FG_DEFAULT  = 39,
    ST_INVIS    = 8,
    ST_DEF      = 0,
    ST_BOLD     = 1,
    ST_UNDER    = 4
};

std::string startval(Code code) {
    std::stringstream ss;
    ss << "\033[" << code << "m";
    return ss.str();
}

std::string endval(Code code) {
    std::string str;
    if (code < 30) { str = "\033[0m"; }
    else { str = "\033[39m"; }
    return str;
}

std::string setval(Code code, const std::string& input) {
    std::stringstream ss;
    ss << "\033[" << code << "m" << input << endval(code);
    return ss.str();
}
}

using namespace Color;

namespace LoginTools {

bool check_user(const std::string& username) {
    ObjStore db;
    mariadb::result_set_ref result = db.select("SELECT EXISTS(SELECT * FROM Users WHERE user = '" + username + "');");
    return (bool)result->get_unsigned8(0);
}

bool login(const std::string& username, std::string pass) {
    ObjStore db;
    mariadb::result_set_ref result = db.select("SELECT pwhash FROM Users WHERE user = '" + username + "');");
    std::string pwhash = result->get_string(1);
    return Key::verify_key(pwhash, std::move(pass));
}

std::string get_name(const std::string& username){
    ObjStore db;
    mariadb::result_set_ref result = db.select("SELECT name FROM Users WHERE user = '" + username + "');");
    return result->get_string(0);
}

}

void login_menu() {
    std::string username;
    bool user_success = false, pass_success = false;
    std::cout << "--------------------------------" << std::endl << "Welcome to the USW Cyber Lab.\n";
    while (!user_success) {
        std::cout << setval(ST_BOLD, "Username: ");
        std::getline(std::cin, username);
        if (!LoginTools::check_user(username)) {
            std::cout << std::endl << setval(FG_RED, "🗙") << " Username not found. Please try again.\n";
        } else {
            user_success = true;
        }
    }
    while (!pass_success) {
        std::cout << setval(ST_BOLD, "Password: ") << startval(ST_INVIS);
        std::string password;
        std::getline(std::cin, password);
        if (!LoginTools::login(username, password)) {
            std::cout << std::endl << endval(ST_INVIS);
            std::cout << setval(FG_RED, "🗙") << " Username not found. Please try again.\n";
        } else {
            pass_success = true;
            std::cout << std::endl << endval(ST_INVIS);
            std::string name = LoginTools::get_name(username);
            std::cout << "Welcome, " << name.substr(0, name.find(' ')) << ".";
        }
    }
}
