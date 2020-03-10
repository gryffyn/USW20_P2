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
#include "ObjStore.h"

bool verbose;

#include <ostream>
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        ST_INVIS    = 8,
        ST_DEF      = 0
    };
    std::string setval(Code code, const std::string& input) {
        std::stringstream ss;
        ss << "\033[" << code << "m" << input << "\033[";
        if (code < 30) { ss << "0m"; }
        else { ss << "39m"; }
        return ss.str();
    }
}

void create_user() {
    std::cout << "--------------------------------" << std::endl << "Welcome to the USW Cyber Lab.\n";
    std::cout << "Username: ";

}


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
    Key key("hello");
    std::cout << key.get_key();
    std::string str = key.get_key();
    if (Key::verify_key(str, "hello")) { std::cout << "\n" << Color::setval(Color::FG_GREEN, "✔") << " Password verified!"; }
    std::string str_sub = str.substr(0, 20);
    std::cout << std::endl << str_sub;
    if (!(Key::verify_key(str_sub, "hello"))) { std::cout << "\n" << Color::setval(Color::FG_RED, "✘") << " Password did not verify."; }

    //TODO everything. literally everything

    User user(23, "Evan Penner", "gryffyn", "password");
    db.finalize();
    return 0;
}
