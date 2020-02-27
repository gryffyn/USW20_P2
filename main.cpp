// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <sodium.h>

#include <iostream>

#include "ObjStore.h"
#include "cxxopts.hpp"
#include "libs/Key.h"
#include "Log.h"

bool verbose;

int main(int argc, char **argv) {
    cxxopts::Options options("USWCyberLab",
                             "USW Cyber Lab file storage program");
    options.add_options()("v,verbose","Verbose output");
    auto result = options.parse(argc, argv);
    verbose = result["verbose"].as<bool>();
    Log log;
    if (verbose) {log.write("Verbose output enabled.");}
    log.write("Initializing sodium...");
    sodium_init();
    log.write("Sodium initialized.");
    Key key("hello");
    std::cout << key.get_key();
    log.write("Opening user db");
    ObjStore users("users.sqlite");
    log.write("user db opened");
    users.close_db();
    log.write("user db closed");
    return 0;
}
