// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <sodium.h>

#include <iostream>
#include <vector>

#include "Log.h"
#include "User.h"
#include "DataStore.h"
#include "cxxopts.hpp"
#include "Key.h"

bool verbose;

int main(int argc, char **argv) {
    Log log;
    cxxopts::Options options("USWCyberLab", "USW Cyber Lab file storage program");
    options.add_options()("v,verbose", "Verbose output - shows log levels INFO and WARN")("h,help", "Prints help");
    auto result = options.parse(argc, argv);
    log.verbose = result["verbose"].as<bool>();
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    log.write(Log::INFO, "Initializing sodium...");
    static_cast<void>(sodium_init());
    log.write(Log::INFO, "Sodium initialized.");
    Key key("hello");
    std::cout << key.get_key();
    std::string str = key.get_key();
    std::string str_sub = str.substr(0, 20);
    std::cout << std::endl << str_sub;
    if (Key::verify_key(str_sub, "hello")) { std::cout << "\nPassword verified."; }
    else { log.write(Log::ERR, "nope"); }

    //TODO everything. literally everything

    DataStore storage;
    storage.out << YAML::BeginMap;
    storage.out << YAML::Key << "name";
    storage.out << YAML::Value << "Ryan Braun";
    storage.out << YAML::Key << "position";
    storage.out << YAML::Value << "LF";
    storage.out << YAML::EndMap;


    return 0;
}
