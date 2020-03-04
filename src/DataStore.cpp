//
// Created by gryffyn on 04/03/2020.
//

#include "DataStore.h"
#include <yaml-cpp/yaml.h>
#include <filesystem>
#include "Log.h"
#include "User.h"

namespace fs = std::filesystem;

void DataStore::get_paths() {
    store_path = std::filesystem::current_path() / "users";
}

DataStore::DataStore() {
    Log log;
    if (!fs::exists(store_path)) {
        try {
            fs::create_directory(store_path);
        } catch (...) {
            log.write(Log::ERR, "Error creating user directory.");
        }
    }
}

void DataStore::save_user(User user) {

}

void DataStore::initialize(User user) {

}