// Copyright 2020 Evan Penner
//
// All components are licensed under the MIT License.
// A copy is included in LICENSE

#include <iostream>
#include "key_libs/Key.h"

int main() {
    Key key(";'[,/.,");
    std::cout << key.get_key();
    return 0;
}
