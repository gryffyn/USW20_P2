// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <iostream>
#include "libs/Key.h"
#include <sodium.h>
#include "cxxopts.hpp"
#include "ObjStore.h"

bool verbose,log;

int main(int argc, char** argv) {
    cxxopts::Options options("USWCyberLab", "USW Cyber Lab file storage program");
    options.add_options()
            ("l,log", "Enable logging")
            ("v,verbose", "Verbose output");
    auto result = options.parse(argc, argv);
    verbose = result["verbose"].as<bool>();
    log = result["log"].as<bool>();
    sodium_init();
    Key key("hello");
    std::cout << key.get_key();
    ObjStore users("users.sqlite");
    users.close_db();
    return 0;
}
