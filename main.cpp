// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <sodium.h>

#include <iostream>
#include <vector>

#include "Key.hpp"
#include "Log.hpp"
#include "ObjStore.hpp"
#include "User.hpp"
#include "Student.hpp"
#include "cxxopts.hpp"
#include "UserMenu.hpp"

bool verbose;

int main(int argc, char **argv) {
    ObjStore db;
    db.init_db();
    // Log::write(Log::INFO, "Initializing sodium...");
    static_cast<void>(sodium_init());
    cxxopts::Options options("USWCyberLab", "USW Cyber Lab file storage program");
    options.add_options()("v,verbose", "Verbose output - shows log levels INFO and WARN")("h,help", "Prints help");
    auto result = options.parse(argc, argv);
    verbose = result["verbose"].as<bool>();
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    //TODO everything. literally everything

    // std::cout << "Adding user...";
    // User user(db, "Evan Penner", "gryffyn", "THISISaverysecurepassword23123?!");
    // std::cout << std::endl << "Result = " << user.creation_result << std::endl;

    std::string sql = "SELECT * FROM Users;";
    std::cout << std::endl << sql;
    mariadb::result_set_ref m_result = db.select(sql);
    std::cout << std::endl << m_result->next();
    // std::cout << std::endl << pwhash;
    std::cout << std::endl << m_result->get_string(3);
    std::cout << std::endl << Key::verify_key(m_result->get_string(3), "THISISaverysecurepassword23123?!");
    std::string username = "gryffyn";
    std::string sql2 = "SELECT pwhash FROM Users WHERE user = '" + username + "';";
    mariadb::result_set_ref m2_result = db.select(sql2);
    std::cout << "\nSQL2 - command:\n" << sql2 << "\nExists: " << m2_result->next();
    std::cout << "\nstring = " << m2_result->get_string(0) << "\nINIT LOGIN MENU...";

    Login::login_menu(db);

    db.finalize();
    return 0;
}
