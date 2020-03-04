//
// Created by gryffyn on 04/03/2020.
//

#ifndef USW20_P2_DATASTORE_H
#define USW20_P2_DATASTORE_H

#include <yaml-cpp/yaml.h>

#include <fstream>

#include "User.h"
#include <filesystem>

class DataStore {
   private:
    std::filesystem::path store_path;
    void get_paths();
    std::ofstream user_file;
    void initialize(User user);
    void save_user(User user);

   public:
    DataStore();
    YAML::Emitter out;
};

#endif  // USW20_P2_DATASTORE_H
