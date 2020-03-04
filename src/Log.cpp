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
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[30];
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    strftime(buffer, 30, "%Y%m%d-%H-%M-%S", timeinfo);
    std::stringstream ss;
    ss << buffer;
    logfile.open(("log/" + ss.str() + ".log"), std::ofstream::app);
}

Log::~Log() { logfile.close(); }

void Log::write(level level, const std::string& msg) {
    if (verbose) {
        time_t rawtime;
        struct tm* timeinfo;
        char buffer[30];
        time(&rawtime);
        timeinfo = gmtime(&rawtime);
        strftime(buffer, 30, "%FT%T", timeinfo);
        if (level == 0) {logfile << "[" << buffer << "][INFO]: " << msg << std::endl;}
        else if (level == 1) {logfile << "[" << buffer << "][WARN]: " << msg << std::endl;}
        else if (level == 2) {logfile << "[" << buffer << "][ERR]: " << msg << std::endl;}

    } else {
        if (!(level == 0)) {
            time_t rawtime;
            struct tm* timeinfo;
            char buffer[30];
            time(&rawtime);
            timeinfo = gmtime(&rawtime);
            strftime(buffer, 30, "%FT%T", timeinfo);
            if (level == 1) {logfile << "[" << buffer << "][WARN]: " << msg << std::endl;}
            else if (level == 2) {logfile << "[" << buffer << "][ERR]: " << msg << std::endl;}
        }
    }
}