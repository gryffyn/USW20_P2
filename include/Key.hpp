// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#ifndef USW20_P2_KEY_HPP
#define USW20_P2_KEY_HPP

#include <string>

class Key {
   public:
    explicit Key(std::string pass);
    std::string get_key();
    static bool verify_key(std::string key, std::string pass);
    class pass_exception;

   private:
    std::string finalkey;
};

#endif  // USW20_P2_KEY_HPP
