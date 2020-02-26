// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <iostream>
#include "libs/Key.h"
#include <sodium.h>

int main() {
    sodium_init();
    Key key("hello");
    std::cout << key.get_key();
    return 0;
}
