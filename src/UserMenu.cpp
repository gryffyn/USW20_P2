//
// Created by gryffyn on 10/03/2020.
//

#include "UserMenu.hpp"

#include <mhash.h>

#include <Admin.hpp>
#include <DataTools.hpp>
#include <Lecturer.hpp>
#include <Student.hpp>
#include <User.hpp>
#include <boost/algorithm/string.hpp>
#include <ios>
#include <iostream>
#include <limits>
#include <mariadb++/connection.hpp>
#include <sstream>

#include "Announcement.hpp"
#include "Key.hpp"
#include "ObjStore.hpp"

namespace Color {
enum Code {  // enum of terminal ASCII codes
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
    if (code < 30) {
        str = "\033[0m";
    } else {
        str = "\033[39m";
    }
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
}  // namespace Color

using namespace Color;

// clears console
void clr() { std::cout << "\033[2J\033[1;1H"; }

// removes all characters that aren't alphanum
std::string sanitize_username(std::string input) {
    std::stringstream output;
    for (char c : input) {
        if (isalnum(c)) {
            output << c;
        }
    }
    return output.str();
}

namespace Login {

// checks if user exists and if it does, returns true and the user_id. If not, returns false and -1
std::pair<bool, int> check_user(ObjStore& db, const std::string& username) {
    std::pair<bool, int> retval;
    retval.first = false;
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
    } catch (std::out_of_range& e) {
    }
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

std::pair<std::string, int> login_menu(ObjStore& db) {
    clr();
    std::string username;
    int user_id = 0;
    bool user_success = false, pass_success = false;
    std::string password;
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
                std::cout << setval(ST_BOLD, "Password: ");
                std::getline(std::cin, password);
                if (!login(db, user_id, password)) {
                    std::cout << std::endl;
                    std::cout << setval(FG_RED, "🗙") << " Password incorrect. Please try again.\n";
                } else {
                    pass_success = true;
                    std::string name = get_name(db, user_id);
                    std::cout << "Logging in....." << std::endl;
                    // sleep(1);
                    clr();
                    std::cout << "Welcome, " << name.substr(0, name.find(' ')) << ".";
                }
            }
        }
    }
    std::pair<std::string, int> final;
    final.first = password;
    final.second = user_id;
    return final;
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
    if (result->next()) {
        return true;
    } else { return false; }
}

// checks the user type by looking in what table(s) the user_id exists, returns the types
std::vector<std::string> check_user_type(ObjStore& db, const int& user_id) {
    std::vector<std::string> user_types{"Student", "Lecturer", "Admin"}, output;
    std::stringstream sql;
    for (std::string type : user_types) {
        sql << "SELECT * from " << type << "s WHERE user_id = " << user_id << ";";
        if (exists_row(db, sql.str())) {
            output.emplace_back(type);
        }
        std::stringstream().swap(sql);
    }
    return output;
}

std::string set_user_type(const int& choice) {
    if (choice == 1) {
        return "Student";
    } else if (choice == 2) {
        return "Lecturer";
    } else if (choice == 3) {
        return "Admin";
    } else {
        std::cout << "Invalid choice.";
        return "";
    }
}

void create_user(ObjStore& db) {
    std::cout << make_box("Create User");
    std::string username, name, password, typechoice;
    std::vector<int> typechoices;
    // get user info
    bool user_success = false;
    while (!user_success) {
        username = getstring(setval(ST_BOLD, "Username: "));
        std::pair<bool, int> user_exists = Login::check_user(db, username);
        if (user_exists.first) {
            user_success = true;
        } else {
            std::cout << "Username has already been chosen. Please choose another username.";
        }
    }
    name = getstring(setval(ST_BOLD, "Name: "));
    Key pwhash;
    bool pass_success{};
    while (!pass_success) {
        try {
            pwhash.create_key(getstring(setval(ST_BOLD, "Password: ")));
            pass_success = true;
        } catch (Key::pass_exception& e) {
            std::cout << e.what();
        }
    }
    std::cout << setval(ST_BOLD, "User type(s):\n")
              << "1. Student\n"
              << "2. Lecturer\n"
              << "3. Admin\n";
    typechoice = getstring("Enter comma separated list of user types: ");
    for (char c : typechoice) {
        if (isdigit(c)) {
            typechoices.emplace_back(c - '0');
        }
    }
    std::cout << "\nCreating user...";
    for (int i : typechoices) {
        if (i == 1) {
            Student student(db, name, username, pwhash.get_key());
        } else if (i == 2) {
            Lecturer lecturer(db, name, username, pwhash.get_key());
        } else if (i == 3) {
            Admin admin(db, name, username, pwhash.get_key());
        }
    }
    std::cout << "\nUser created.";
}

void list_users(ObjStore& db) {
    std::cout << make_box("User List");
    std::cout << setval(ST_UNDER, setval(ST_BOLD, "Username"));
    std::cout << "  " << setval(ST_BOLD, "-") << "  ";
    std::cout << setval(ST_UNDER, setval(ST_BOLD, "Name")) << std::endl;
    mariadb::result_set_ref result = db.select("SELECT user, name FROM Users;");
    while (result->next()) {
        std::cout<< result->get_string("user") << "  -  " << result->get_string("name") << std::endl;
    }
}

void delete_user(ObjStore& db) {
    std::cout << make_box("Delete User");
    std::string username = getstring("Username: ");
    std::cout << "\nSelected username: " << username;
    std::string confirm = getstring("Are you sure you want to delete this user?\nYou can't undo this action. (y/N)");
    if (!confirm.empty()) { boost::algorithm::to_lower(confirm); }
    if (confirm[0] == 'n' || confirm.empty()) {
        std::cout << setval(ST_BOLD, "NOT") << " deleting user \"" << username << "\".";
    } else if (confirm[0] == 'y') {
        std::cout << "Deleting user \"" << username << "\"...";
        db.execute("DELETE FROM Users WHERE user = '" + username + "';");
        std::cout << std::endl << "User \"" << username << "\" deleted.";
    }
}

void show_data(ObjStore& db, const int& user_id, const std::string& user_type, std::string password) {
    std::stringstream sql;
    sql << "SELECT data FROM " << user_type << "s WHERE user_id = " << user_id << ";";
    std::string sql_s(sql.str());
    mariadb::result_set_ref result = db.select(sql.str());
    result->next();
    std::cout << make_box("Data");
    std::string data = result->get_string(0);
    if (data.empty()) {
        std::cout << setval(FG_RED, "🗙") << "No data found.";
    } else {
        std::cout << DataTools::get_data_xor(db, user_id, user_type, std::move(password));
    }
}

void edit_data(ObjStore& db, const int& user_id, const std::string& user_type, std::string password) {
    std::cout << make_box("Data");
    std::stringstream sql;
    std::string data;
    std::string choice;
    int choice_i = 0;
    std::cout << setval(ST_BOLD, "1. ") << "Create data\n";
    std::cout << setval(ST_BOLD, "2. ") << "Add to data\n";
    std::cout << setval(ST_BOLD, "3. ") << "Go back\n";
    std::cout << "\nSelect action: ";
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::getline(std::cin, choice);
    choice_i = stoi(choice);
    if (choice_i == 1) {
        data = getstring("Input data: ");
        DataTools::save_data_xor(db, user_id, user_type, data, std::move(password));
    } else if (choice_i == 2) {
        data = getstring("Data will be added to your existing data.\nInput data: ");
        std::getline(std::cin, data);
        std::string total_data = DataTools::get_data_xor(db, user_id, user_type, std::move(password)) + data;
        sql << "UPDATE " << user_type << "s SET data = '"
            << DataTools::return_data_xor(db, user_id, total_data, std::move(password))
            << "' WHERE user_id = " << user_id << ";";
        db.execute(sql.str());
    }
}

bool back_or_exit() {
    std::cout << "\nGo back (9) or exit (0): ";
    char c;
    std::cin >> c;
    return (c == '9');
}

void show_announcements(ObjStore& db, const int& user_id, std::string user_type) {
    std::cout << make_box("Announcements");
    std::stringstream sql, sql2;
    std::string data;
    std::string choice;
    int choice_i = 0;
    std::cout << setval(ST_BOLD, "1. ") << "Show unread announcements\n";
    std::cout << setval(ST_BOLD, "2. ") << "Show announcement by ID\n";
    std::cout << setval(ST_BOLD, "3. ") << "Go back\n";
    std::cout << "\nSelect action: ";
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::getline(std::cin, choice);
    choice_i = stoi(choice);
    if (choice_i == 1) {
        clr();
        std::cout << make_box("Announcements");
        sql << "SELECT last_notif FROM " << user_type << "s WHERE user_id = " << user_id << ";";
        mariadb::result_set_ref result = db.select(sql.str());
        result->next();
        mariadb::date_time last = result->get_date_time(0);
        sql2 << "SELECT * FROM Announcements WHERE ann_time >= '" << last << "';";
        mariadb::result_set_ref result2 = db.select(sql2.str());
        while (result2->next()) {
            std::stringstream time, sql3;
            sql3 << "SELECT name FROM Users WHERE user_id = " << result2->get_string("ann_author") << ";";
            mariadb::result_set_ref result3 = db.select(sql3.str());
            result3->next();
            time << result2->get_date_time("ann_time");
            std::cout << std::endl << setval(ST_BOLD, result2->get_string("ann_title"))
                      << std::endl << result3->get_string("name") << std::endl
                      << setval(ST_UNDER, time.str()) << std::endl
                      << result->get_string("ann_text");
        }
        std::stringstream newtime;
        newtime << "UPDATE " << user_type << "s SET last_notif = '" << mariadb::date_time::now_utc() << "' WHERE user_id = "
                << user_id << ";";
        db.insert(newtime.str());
        std::cout << std::endl << "\nPress enter to continue...";
        std::cin.ignore();
    } else if (choice_i == 2) {
        std::string ann_number = getstring("Enter announcement number: ");
        /* std::stringstream ss;
        for (char c : ann_number) {
            if (isdigit(c)) {
                ss << (c + '0');
            }
        } */
        int ann_id = stoi(ann_number);
        // Announcement ann(db);
        std::stringstream ss;
        ss << "SELECT * FROM Announcements WHERE ann_id = " << ann_id << ";";
        try {
            mariadb::result_set_ref result = db.select(ss.str());
            result->next();
            clr();
            std::stringstream sql3;
            sql3 << "SELECT name FROM Users WHERE user_id = " << result->get_string("ann_author") << ";";
            mariadb::result_set_ref result3 = db.select(sql3.str());
            result3->next();
            std::cout << make_box("Announcement");
            std::stringstream time2;
            time2 << result->get_date_time("ann_time");
            std::cout << std::endl << setval(ST_BOLD, result->get_string("ann_title"))
                      << std::endl << result3->get_string("name") << std::endl
                      << setval(ST_UNDER, time2.str()) << std::endl
                      << result->get_string("ann_text");
            std::cout << std::endl << "\nPress enter to continue...";
            std::cin.ignore();
        } catch (std::out_of_range& e) {
            std::cout << std::endl << "No announcement found with that number.";
            std::cout << std::endl << "Press enter to continue...";
            std::cin.ignore();
        }
    }
}

void create_announcement(ObjStore& db, const int& user_id) {
    make_box("Create Announcement");
    std::string title,text;
    title = getstring("Title: ");
    text = getstring("Text: ");
    Announcement ann(db, user_id, title, text);
    std::cout << std::endl << "Announcement created.\nPress enter to continue...";
    std::cin.ignore();
}

bool menu_admin(ObjStore& db, int& count, const int& user_id, std::string user_type) {
    std::string choice;
    int choice_i = 0;
    std::cout << setval(ST_BOLD, "1. ") << "List Users\n";
    std::cout << setval(ST_BOLD, "2. ") << "Create User\n";
    std::cout << setval(ST_BOLD, "3. ") << "Delete User\n";
    std::cout << setval(ST_BOLD, "4. ") << "Show announcements\n";
    std::cout << setval(ST_BOLD, "5. ") << "Create announcement\n";
    std::cout << setval(ST_BOLD, "6. ") << "Exit\n";
    std::cout << "\nSelect action: ";
    if (count != 0) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); }
    std::getline(std::cin, choice);
    choice_i = stoi(choice);
    if (choice_i == 1) {
        clr();
        list_users(db);
        if (!back_or_exit()) { return true; } else { clr(); return false; } // exits, or clears the screen and shows menu again
    } else if (choice_i == 2) {
        clr();
        create_user(db);
        if (!back_or_exit()) { return true; } else { clr(); return false; }
    } else if (choice_i == 3) {
        clr();
        delete_user(db);
        if (!back_or_exit()) { return true; } else { clr(); return false; }
    } else if (choice_i == 4) {
        clr();
        show_announcements(db, user_id, user_type);
        clr();
        return false;
    } else if (choice_i == 5) {
        clr();
        create_announcement(db, user_id);
        clr();
        return false;
    }
    else { return true; }
}

bool menu_student(ObjStore& db, int& count, const int& user_id, const std::string& student, std::string password) {
    std::string choice;
    int choice_i = 0;
    std::cout << setval(ST_BOLD, "1. ") << "Show data\n";
    std::cout << setval(ST_BOLD, "2. ") << "Set/edit data\n";
    std::cout << setval(ST_BOLD, "3. ") << "Show announcements\n";
    std::cout << setval(ST_BOLD, "4. ") << "Exit\n";
    std::cout << "\nSelect action: ";
    if (count != 0) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); }
    std::getline(std::cin, choice);
    choice_i = stoi(choice);
    if (choice_i == 1) {
        clr();
        show_data(db, user_id, student, password);
        if (!back_or_exit()) { return true; } else { clr(); return false;}
    } else if (choice_i == 2) {
        clr();
        edit_data(db, user_id, student, password);
        if (!back_or_exit()) { return true; } else { clr(); return false; }
    } else if (choice_i == 3) {
        clr();
        show_announcements(db, user_id, student);
        clr();
        return false;
    } else { return true; }
}

bool menu_lecturer(ObjStore& db, int& count, const int& user_id, const std::string& lecturer, std::string password) {
    std::string choice;
    int choice_i = 0;
    std::cout << setval(ST_BOLD, "1. ") << "Show data\n";
    std::cout << setval(ST_BOLD, "2. ") << "Set/edit data\n";
    std::cout << setval(ST_BOLD, "3. ") << "Show announcements\n";
    std::cout << setval(ST_BOLD, "4. ") << "Create announcement\n";
    std::cout << setval(ST_BOLD, "5. ") << "Exit\n";
    std::cout << "\nSelect action: ";
    if (count != 0) { std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n'); }
    std::getline(std::cin, choice);
    choice_i = stoi(choice);
    if (choice_i == 1) {
        clr();
        show_data(db, user_id, lecturer, password);
        if (!back_or_exit()) { return true; } else { clr(); return false; }
    } else if (choice_i == 2) {
        clr();
        edit_data(db, user_id, lecturer, password);
        if (!back_or_exit()) { return true; } else { clr(); return false; }
    } else if (choice_i == 3) {
        clr();
        show_announcements(db, user_id, lecturer);
        clr();
        return false;
    } else if (choice_i == 4) {
        clr();
        create_announcement(db, user_id);
        clr();
        return false;
    }
    else { return true; }
}

void show_menu(ObjStore& db, const int& user_id, std::string password) {
    std::vector<std::string> user_type = check_user_type(db, user_id);
    std::cout << std::endl << make_box("Menu Options:") << std::endl;
    bool exit = false;
    int count = 0;
    while (!exit) {
        if (std::find(user_type.begin(), user_type.end(), "Admin") != user_type.end()) {
            exit = menu_admin(db, count, user_id, "Admin");
        } else if (std::find(user_type.begin(), user_type.end(), "Lecturer") != user_type.end()) {
            exit = menu_lecturer(db, count, user_id, "Lecturer", password);
        } else if (std::find(user_type.begin(), user_type.end(), "Student") != user_type.end()) {
            exit = menu_student(db, count, user_id, "Student", password);
        }
        ++count;
    }
}

}  // namespace UserMenu