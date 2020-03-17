//
// Created by gryffyn on 18/02/2020.
//

#include "Student.hpp"

#include <sodium.h>
#include <mariadb++/connection.hpp>
#include <utility>
#include <string>

#include "Xor.hpp"
#include "ObjStore.hpp"

using namespace mariadb;

Student::Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    db.execute("INSERT INTO Students(user_id) VALUES ((SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1))");
}

Student::Student(ObjStore& u_db, const std::string& name, const std::string& user, const std::string& pwhash, std::string password,
                 std::string data)
    : User(u_db, name, user, pwhash) {
    db = u_db;
    mariadb::result_set_ref result = db.select("SELECT user_id FROM Users ORDER BY user_id DESC LIMIT 1;");
    result->next();
    int user_id = result->get_unsigned16("user_id");
    std::stringstream ss;
    ss << "INSERT INTO Students(user_id) VALUES (" << user_id << ");";
    db.execute(ss.str());
    DataTools::save_data(db, user_id, data, password);
}

namespace DataTools {

void save_data(ObjStore& db, unsigned int userid, std::string data, std::string passwd) { // not working
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
    ss << "UPDATE Students SET data = '" << nonce << enc_data << "' WHERE user_id = '" << userid << "';";
    db.execute(ss.str());
}

void save_data_xor(ObjStore& db, unsigned int userid, std::string data, std::string passwd) {
    std::stringstream ss, ss2;
    ss2 << "SELECT pwhash FROM Users WHERE user_id = " << userid << ";";
    mariadb::result_set_ref result = db.select(ss2.str());
    result->next();
    std::string pwhash = result->get_string("pwhash");
    std::string derived = KeyDer::create_derived(pwhash, std::move(passwd));
    std::string enc_data = Xor::xor_string(derived, data);
    ss << "UPDATE Students SET data = '" << enc_data << "' WHERE user_id = '" << userid << "';";
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

std::string get_data(ObjStore& db, unsigned int userid, std::string passwd) { // not working
    std::stringstream ss, ss2;
    unsigned char* dec_data{};
    ss << "SELECT user_id, data FROM Students WHERE user_id = '" << userid << "';";
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

std::string get_data_xor(ObjStore& db, unsigned int userid, std::string passwd) {
    std::stringstream ss, ss2;
    ss2 << "SELECT pwhash FROM Users WHERE user_id = " << userid << ";";
    mariadb::result_set_ref result = db.select(ss2.str());
    result->next();
    std::string pwhash = result->get_string("pwhash");
    ss << "SELECT user_id, data FROM Students WHERE user_id = '" << userid << "';";
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

