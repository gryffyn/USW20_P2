// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <sodium.h>

#include <Student.hpp>
#include <iostream>
#include <vector>

#include "Admin.hpp"
#include "Key.hpp"
#include "Log.hpp"
#include "ObjStore.hpp"
#include "User.hpp"
#include "UserMenu.hpp"
#include "cxxopts.hpp"

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
    // TODO everything. literally everything
    // default admin account
    // Admin admin(db, "Default Admin", "admin", "$argon2id$v=19$m=65535,t=19,p=1$UkFORE9NU0FMVA$IRwZGDV0n+KpsCd2To1QXxkIuc2Z439PT9yuhbnA8CM");
    std::pair<std::string, int> loginuser = Login::login_menu(db);
    UserMenu::show_menu(db, loginuser.second, loginuser.first);
    db.finalize();
    return 0;
}
