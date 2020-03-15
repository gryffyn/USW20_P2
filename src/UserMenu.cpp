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

namespace Login {

std::pair<bool, int> check_user(ObjStore& db, const std::string& username) {
    std::pair<bool, int> retval;
    try {
        mariadb::result_set_ref user_result = db.select("SELECT user_id, user FROM Users WHERE user = '" + username + "';");
        user_result->next();
        if (username == user_result->get_string(1)) {
            retval.first = true; retval.second = user_result->get_unsigned16(0);
        }
    } catch (std::out_of_range& e){
        std::cout << e.what();
        retval.second = -1;
    }
    return retval;
}

bool login(ObjStore& db, const int& user_id, std::string pass) {
    bool valid = false;
    mariadb::result_set_ref pass_result;
    std::stringstream sql;
    sql << "SELECT user_id, pwhash FROM Users WHERE user_id = " << user_id << ";";
    try {
        pass_result = db.select(sql.str());
        pass_result->next();
        std::string pwhash = pass_result->get_string(1);
        valid = Key::verify_key(pwhash, pass);
    } catch (std::out_of_range& e) {
        std::cout << e.what();
    }
    return valid;
}

std::string get_name(ObjStore db, const int& user_id){
    std::stringstream sql;
    sql << "SELECT user_id, name FROM Users WHERE user_id = " << user_id << ";";
    mariadb::result_set_ref result = db.select(sql.str());
    result->next();
    return result->get_string(1);
}

void login_menu(ObjStore& db) {
    std::string username;
    int user_id = 0;
    bool user_success = false, pass_success = false;
    std::cout << "--------------------------------" << std::endl << "Welcome to the USW Cyber Lab.\n";
    while (!user_success) {
        std::cout << setval(ST_BOLD, "Username: ");
        std::getline(std::cin, username);
        std::pair<bool, int> check = check_user(db, username);
        if (!check.first) {
            std::cout << std::endl << setval(FG_RED, "🗙") << " Username not found. Please try again.\n";
        } else {
            user_id = check.second;
            user_success = true;
            while (!pass_success) {
                std::cout << setval(ST_BOLD, "Password: ") << startval(ST_INVIS);
                std::string password;
                std::getline(std::cin, password);
                if (!login(db, user_id, password)) {
                    std::cout << std::endl << endval(ST_INVIS);
                    std::cout << setval(FG_RED, "🗙") << " Password incorrect. Please try again.\n";
                } else {
                    pass_success = true;
                    std::cout << std::endl << endval(ST_INVIS);
                    std::string name = get_name(db, user_id);
                    std::cout << "Welcome, " << name.substr(0, name.find(' ')) << ".";
                }
            }
        }
    }
}

}