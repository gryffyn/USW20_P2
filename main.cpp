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
#include "cxxopts.hpp"

bool verbose;

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
    /* Key key("hello");
    std::cout << key.get_key();
    std::string str = key.get_key();
    if (Key::verify_key(str, "hello")) { std::cout << "\n" << Color::setval(Color::FG_GREEN, "✔") << " Password verified!"; }
    std::string str_sub = str.substr(0, 20);
    std::cout << std::endl << str_sub;
    if (!(Key::verify_key(str_sub, "hello"))) { std::cout << "\n" << Color::setval(Color::FG_RED, "✘") << " Password did not verify."; }
    */

    //TODO everything. literally everything
    std::cout << "Adding user...";
    User user(db, "Evan Penner", "gryffyn", "THISISaverysecurepassword23123?!");
    std::cout << std::endl << "Result = " << user.creation_result;
    db.finalize();
    return 0;

}
