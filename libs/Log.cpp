//
// Created by gryffyn on 2/26/20.
//

#include "Log.h"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

Log::Log() {
    fs::path logdir = fs::current_path() / "log";
    if (!fs::exists(logdir)) {
        try {
            fs::create_directory(logdir);
        } catch (...) {
            std::cout << "Error creating log directory.";
        }
    }
    time_t timer;
    time(&timer);
    std::ofstream logfile("");
}

void Log::write(const std::string &msg) {}