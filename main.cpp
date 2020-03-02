// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <sodium.h>

#include <iostream>
#include <vector>

#include "Log.h"
#include "ObjStore.h"
#include "User.h"
#include "cxxopts.hpp"
#include "Key.h"
#include <mysql/mariadb_com.h>

bool verbose;
`
int main(int argc, char **argv) {
    Log log;
    cxxopts::Options options("USWCyberLab", "USW Cyber Lab file storage program");
    options.add_options()("v,verbose", "Verbose output")("h,help", "Prints help");
    auto result = options.parse(argc, argv);
    verbose = result["verbose"].as<bool>();
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    if (verbose) {
        log.write("Verbose output enabled.");
    }
    log.write("Initializing sodium...");
    static_cast<void>(sodium_init());
    log.write("Sodium initialized.");
    Key key("hello");
    std::cout << key.get_key();
    // init db
    log.write("Opening user db");
    ObjStore users("users.sqlite");
    log.write("user db opened");
    users.close_db();
    log.write("user db closed");
    if (Key::verify_key(key.get_key(), "hello")) { std::cout << "\nPassword verified."; }

    //TODO everything. literally everything

    return 0;
}
