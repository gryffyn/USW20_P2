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

bool verbose;


int main(int argc, char **argv) {
    ObjStore db;
    db.init_db();
    cxxopts::Options options("USWCyberLab", "USW Cyber Lab file storage program");
    options.add_options()("v,verbose", "Verbose output - shows log levels INFO and WARN")("h,help", "Prints help");
    auto result = options.parse(argc, argv);
    verbose = result["verbose"].as<bool>();
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    // Log::write(Log::INFO, "Initializing sodium...");
    static_cast<void>(sodium_init());
    // Log::write(Log::INFO, "Sodium initialized.");

    //TODO everything. literally everything

    std::cout << "Adding user...";
    Student user(db, "Evan Penner", "gryffyn", "THISISaverysecurepassword23123?!");
    user.save_data()
    std::cout << std::endl << "Result = " << user.creation_result;
    db.finalize();
    return 0;

}
