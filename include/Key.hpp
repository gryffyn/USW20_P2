// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#ifndef USW20_P2_KEY_HPP
#define USW20_P2_KEY_HPP

#include <string>

namespace KeyDer {
std::string create_derived(std::string pwhash, std::string password);
}

class Key {
   public:
    explicit Key();
    explicit Key(std::string pass);
    void create_key(std::string pass);
    std::string get_key();
    static bool verify_key(std::string key, std::string pass);
    bool verify_key(std::string pass);
    class pass_exception : public std::exception {
       public:
        [[nodiscard]] const char* what() const noexcept override { return "Password is invalid length/strength."
                                                                          "\nPassword is required to have:\n"
                                                                          " • >= 14 characters\n"
                                                                          " • >= 1 uppercase letter\n"
                                                                          " • >= 1 digit\n"
                                                                          " • >= 1 symbol (except \\)"; }
    };

   private:
    std::string finalkey{};
};

#endif  // USW20_P2_KEY_HPP
