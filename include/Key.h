// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#ifndef USW20_P2_KEY_H
#define USW20_P2_KEY_H

#include <string>

class Key {
   public:
    explicit Key(const std::string& pass);
    std::string get_key();
    static bool verify_key(std::string key, std::string pass);

   private:
    std::string finalkey;
};

#endif  // USW20_P2_KEY_H
