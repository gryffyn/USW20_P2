// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <iostream>
#include "libs/Key.h"

int main() {
    Key key("hello");
    std::cout << key.get_key();
    return 0;
}
