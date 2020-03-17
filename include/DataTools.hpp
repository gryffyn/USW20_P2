//
// Created by gryffyn on 17/03/2020.
//

#ifndef USW20_P2_DATATOOLS_HPP
#define USW20_P2_DATATOOLS_HPP

#include <string>
#include "ObjStore.hpp"

namespace DataTools {
void save_data(ObjStore& db, unsigned int userid, std::string user_type, std::string data, std::string password);
std::string get_data(ObjStore& db, unsigned int userid, std::string user_type, std::string password);
std::string get_data_xor(ObjStore& db, unsigned int userid, std::string user_type, std::string passwd);
void save_data_xor(ObjStore& db, unsigned int userid, std::string user_type, std::string data, std::string passwd);
std::string return_data_xor(ObjStore& db, unsigned int userid, std::string data, std::string passwd);
}

#endif  // USW20_P2_DATATOOLS_HPP
