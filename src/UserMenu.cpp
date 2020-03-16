//
// Created by gryffyn on 10/03/2020.
//

#include "UserMenu.hpp"

#include <mhash.h>

#include <Admin.hpp>
#include <Lecturer.hpp>
#include <Student.hpp>
#include <User.hpp>
#include <iostream>
#include <mariadb++/connection.hpp>
#include <sstream>

#include "Key.hpp"
#include "ObjStore.hpp"

namespace Color {
enum Code {    // enum of terminal ASCII codes
    FG_RED = 31,
    FG_GREEN = 32,
    FG_BLUE = 34,
    FG_DEFAULT = 39,
    ST_INVIS = 8,
    ST_DEF = 0,
    ST_BOLD = 1,
    ST_UNDER = 4
};
// sets text style to given code
std::string startval(Code code) {
    std::stringstream ss;
    ss << "\033[" << code << "m";
    return ss.str();
}
// resets text style to normal
std::string endval(Code code) {
    std::string str;
    if (code < 30) { str = "\033[0m"; }
    else { str = "\033[39m"; }
    return str;
}
// sets input to whatever code
std::string setval(Code code, const std::string& input) {
    std::stringstream ss;
    ss << "\033[" << code << "m" << input << endval(code);
    return ss.str();
}

std::string make_box(std::string msg) {
    std::stringstream box_u, box_l, box_m, box_complete;
    box_u << "┏━";
    box_m << "┃ ";
    box_l << "┗━";
    for (int i = 0; i < msg.length(); i++) {
        box_u << "━";
        box_l << "━";
    }
    box_u << "━┓";
    box_m << setval(ST_BOLD, msg) << " ┃";
    box_l << "━┛";
    box_complete << box_u.str() << "\n" << box_m.str() << "\n" << box_l.str() << "\n";
    return box_complete.str();
}
} // namespace Color

using namespace Color;

// clears console
void clr() { std::cout << "\033[2J\033[1;1H"; }

// removes all characters that aren't alphanum
std::string sanitize_username(std::string input) {
    std::stringstream output;
    for (char c : input) {
        if (isalnum(c)) { output << c; }
    }
    return output.str();
}

namespace Login {

// checks if user exists and if it does, returns true and the user_id. If not, returns false and -1
std::pair<bool, int> check_user(ObjStore& db, const std::string& username) {
    std::pair<bool, int> retval;
    try {
        mariadb::result_set_ref user_result =
            db.select("SELECT user_id, user FROM Users WHERE user = '" + username + "';");
        user_result->next();
        if (username == user_result->get_string(1)) {
            retval.first = true;
            retval.second = user_result->get_unsigned16(0);
        }
    } catch (std::out_of_range& e) {
        retval.second = -1;
    }
    return retval;
}

// checks password
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
    } catch (std::out_of_range& e) {}
    return valid;
}

// gets name based on user_id
std::string get_name(ObjStore& db, const int& user_id) {
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
    std::cout << make_box("Welcome to the USW Cyber Lab.");
    while (!user_success) {
        std::cout << setval(ST_BOLD, "Username: ");
        std::getline(std::cin, username);
        std::string san_uname = sanitize_username(username);
        std::pair<bool, int> check = check_user(db, san_uname);
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
                    std::cout << "Logging in....." << std::endl;
                    sleep(1);
                    clr();
                    std::cout << "Welcome, " << name.substr(0, name.find(' ')) << ".";
                }
            }
        }
    }
}

}  // namespace Login

namespace UserMenu {

std::string getstring(std::string output) {
    std::string str;
    std::cout << output;
    std::getline(std::cin, str);
    return str;
}

// returns if row exists or not
bool exists_row(ObjStore& db, std::string sql) {
    mariadb::result_set_ref result = db.select(sql);
    result->next();
    return result->get_boolean(0);
}

// checks the user type by looking in what table(s) the user_id exists, returns the types
std::vector<std::string> check_user_type(ObjStore& db, const int& user_id) {
    std::vector<std::string> user_types{"Student", "Lecturer", "Admin"}, output;
    std::stringstream sql;
    for (std::string type : user_types) {
        sql << "SELECT EXISTS (SELECT * from " << type << "s WHERE user_id = " << user_id << ");";
        if (exists_row(db, sql.str())) {
            output.emplace_back(type);
        }
        std::stringstream().swap(sql);
    }
    return output;
}

void show_menu(ObjStore& db, const int& user_id) {
    std::vector<std::string> user_type = check_user_type(db, user_id);
    std::cout << "\nMenu Options:\n";
    if (std::find(user_type.begin(), user_type.end(), "Admin") != user_type.end()) {
        std::cout << setval(ST_BOLD, "1. ") << "List Users";
        std::cout << setval(ST_BOLD, "2. ") << "Create User";
        std::cout << setval(ST_BOLD, "1. ") << "Delete User";
        std::cout << setval(ST_BOLD, "1. ") << "List Users";
    }
}

std::string set_user_type(const int& choice) {
    if (choice == 1) { return "Student"; }
    else if (choice == 2) { return "Lecturer"; }
    else if (choice == 3) { return "Admin"; }
    else { std::cout << "Invalid choice."; return ""; }
}

void create_user(ObjStore& db) {
    std::cout <<make_box(setval(ST_BOLD, "Create User"));
    std::string username, name, password, typechoice;
    std::vector<int> typechoices;
    // get user info
    username = getstring(setval(ST_BOLD, "Username: "));
    name = getstring(setval(ST_BOLD, "\nName: "));
    Key pwhash;
    bool pass_success{};
    while (!pass_success) {
        try {
            pwhash.create_key(getstring(setval(ST_BOLD, "\nPassword: ")));
            pass_success = true;
        } catch (Key::pass_exception& e) {
            std::cout << e.what();
        }
    }
    std::cout << "\n" << setval(ST_BOLD, "User type(s):") << "\n1. Student\n" << "2. Lecturer\n" << "3. Admin\n";
    typechoice = getstring("Enter comma separated list of user types: ");
    for (char c : typechoice) {
        if (isalpha(c)) {
            typechoices.emplace_back(c);
        }
    }
    std::cout << "\nCreating user...";
    for (int i : typechoices) {
        if (i == 1) { Student student(db, name, username, pwhash.get_key()); }
        else if (i == 2) { Lecturer lecturer(db, name, username, pwhash.get_key()); }
        else if (i == 3) { Admin admin(db, name, username, pwhash.get_key()); }
    }
    std::cout << "\nUser created.";
}

}