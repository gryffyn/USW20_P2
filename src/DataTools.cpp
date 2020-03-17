//
// Created by gryffyn on 17/03/2020.
//

#include "DataTools.hpp"

#include "Key.hpp"
#include "sodium.h"
#include <mariadb++/connection.hpp>
#include "Xor.hpp"

using namespace mariadb;

namespace DataTools {

void save_data(ObjStore& db, unsigned int userid, std::string user_type, std::string data, std::string passwd) { // not working
    std::stringstream ss, ss2;
    ss2 << "SELECT pwhash FROM Users WHERE user_id = " << userid << ";";
    mariadb::result_set_ref result = db.select(ss2.str());
    result->next();
    std::string pwhash = result->get_string("pwhash");
    std::string derived = KeyDer::create_derived(pwhash, std::move(passwd));
    const auto* key = reinterpret_cast<const unsigned char*>(derived.c_str());
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char* enc_data{};
    randombytes_buf(nonce, sizeof nonce);
    crypto_secretbox_easy(enc_data, (const unsigned char *)data.c_str(),
                          (crypto_secretbox_MACBYTES + data.length()), nonce, key);
    ss << "UPDATE " << user_type << "s SET data = '" << nonce << enc_data << "' WHERE user_id = '" << userid << "';";
    db.execute(ss.str());
}

void save_data_xor(ObjStore& db, unsigned int userid, std::string user_type, std::string data, std::string passwd) {
    std::stringstream ss, ss2;
    ss2 << "SELECT pwhash FROM Users WHERE user_id = " << userid << ";";
    mariadb::result_set_ref result = db.select(ss2.str());
    result->next();
    std::string pwhash = result->get_string("pwhash");
    std::string derived = KeyDer::create_derived(pwhash, std::move(passwd));
    std::string enc_data = Xor::xor_string(derived, data);
    ss << "UPDATE " << user_type << "s SET data = '" << enc_data << "' WHERE user_id = '" << userid << "';";
    db.execute(ss.str());
}


std::string return_data_xor(ObjStore& db, unsigned int userid, std::string data, std::string passwd) {
    std::stringstream ss, ss2;
    ss2 << "SELECT pwhash FROM Users WHERE user_id = " << userid << ";";
    mariadb::result_set_ref result = db.select(ss2.str());
    result->next();
    std::string pwhash = result->get_string("pwhash");
    std::string derived = KeyDer::create_derived(pwhash, std::move(passwd));
    return Xor::xor_string(derived, data);
}

std::string get_data(ObjStore& db, unsigned int userid, std::string user_type, std::string passwd) { // not working
    std::stringstream ss, ss2;
    unsigned char* dec_data{};
    ss << "SELECT user_id, data FROM " << user_type << "s WHERE user_id = '" << userid << "';";
    result_set_ref result = db.select(ss.str());
    result->next();
    std::string enc_data = result->get_string("data");
    if (enc_data == "NULL") {
        return "NULL";
    } else {
        const auto* f_enc_data = reinterpret_cast<const unsigned char*>(enc_data.substr(crypto_secretbox_NONCEBYTES, enc_data.length()).c_str());
        ss2 << "SELECT pwhash FROM Users WHERE user_id = " << userid << ";";
        mariadb::result_set_ref result = db.select(ss2.str());
        result->next();
        std::string pwhash = result->get_string("pwhash");
        std::string derived = KeyDer::create_derived(pwhash, std::move(passwd));
        const auto* key = reinterpret_cast<const unsigned char*>(derived.c_str());
        const auto* nonce = reinterpret_cast<const unsigned char*>(enc_data.substr(0, crypto_secretbox_NONCEBYTES).c_str());
        (void)crypto_secretbox_open_easy(dec_data, f_enc_data, enc_data.length(), nonce, key);
        std::stringstream final;
        final << dec_data;
        return final.str();
    }
}

std::string get_data_xor(ObjStore& db, unsigned int userid, std::string user_type, std::string passwd) {
    std::stringstream ss, ss2;
    ss2 << "SELECT pwhash FROM Users WHERE user_id = " << userid << ";";
    mariadb::result_set_ref result = db.select(ss2.str());
    result->next();
    std::string pwhash = result->get_string("pwhash");
    ss << "SELECT user_id, data FROM " << user_type << "s WHERE user_id = '" << userid << "';";
    result_set_ref result2 = db.select(ss.str());
    result2->next();
    std::string enc_data = result2->get_string("data");
    if (enc_data.empty()) {
        return "";
    } else {
        std::string derived = KeyDer::create_derived(pwhash, std::move(passwd));
        return Xor::xor_string(derived, enc_data);
    }

}

} // namespace DataTools