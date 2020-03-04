//
// Created by gryffyn on 04/03/2020.
//

#include "TxtStore.h"

#include <Log.h>
#include <User.h>

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
fs::path store_path = fs::current_path() / "users";

TxtStore::TxtStore() {
    Log log;
    if (!fs::exists(store_path)) {
        try {
            fs::create_directory(store_path);
        } catch (...) {
            log.write(Log::ERR, "Error creating user directory.");
        }
    }
}

void TxtStore::initialize(User user) {
    fs::path user_path = store_path / ((std::string)user.ID + ".txt");
    if (!fs::exists(user_path)) {
        user_file.open(user_path, std::ios::trunc);
    }
}

void TxtStore::save_user(User user) {

}