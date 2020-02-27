//
// Created by gryffyn on 2/26/20.
//

#include "Log.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
using namespace boost::posix_time;

Log::Log() {
    fs::path logdir = fs::current_path() / "log";
    if (!fs::exists(logdir)) {
        try {
            fs::create_directory(logdir);
        } catch (...) {
            std::cout << "Error creating log directory.";
        }
    }
    timeUTC = second_clock::universal_time();
    std::stringstream ss;
    ss.imbue(std::locale(std::cout.getloc(), file_facet));
    ss << timeUTC;
    logfile.open(("log/" + ss.str() + ".log"), std::ofstream::app);
}

Log::~Log() { logfile.close(); }

void Log::write(const std::string& msg) {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[30];
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    strftime(buffer, 30, "%FT%T", timeinfo);
    /* timeUTC = microsec_clock::universal_time();
    std::stringstream ss;
    ss.imbue(std::locale(std::cout.getloc(), entry_facet));
    ss << timeUTC; */
    logfile << "[" << buffer << "][INFO]: " << msg << std::endl;
}

void Log::write(level level, const std::string& msg) {
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[30];
    time(&rawtime);
    timeinfo = gmtime(&rawtime);
    strftime(buffer, 30, "%FT%T", timeinfo);
    /* timeUTC = microsec_clock::universal_time();
    std::stringstream ss;
    ss.imbue(std::locale(std::cout.getloc(), entry_facet));
    ss << timeUTC; */
    logfile << "[" << buffer << "][" << level << "]: " << msg << std::endl;
}