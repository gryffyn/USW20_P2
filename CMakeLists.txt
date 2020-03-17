cmake_minimum_required(VERSION 3.15)
project(USW20_P2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS -L/opt/gcc10/lib64)

add_executable(USW20_P2 main.cpp src/Key.cpp src/User.cpp include/User.hpp src/Student.cpp include/Student.hpp
        src/ObjStore.cpp include/ObjStore.hpp src/Log.cpp include/Log.hpp src/UserMenu.cpp include/UserMenu.hpp
        src/Lecturer.cpp include/Lecturer.hpp src/Admin.cpp include/Admin.hpp)

# find directives
find_package(yaml-cpp)
find_package(mariadbclientpp)
find_package(Boost)

# include
include_directories(include/sodium)
include_directories(include)
include_directories(/usr/include/mysql)
include_directories(/usr/include/mysql/mysql)
include_directories(include/mariadbpp)

# target linking
target_link_libraries(${PROJECT_NAME} sodium)
target_link_libraries(${PROJECT_NAME} sqlite3)
target_link_libraries(${PROJECT_NAME} yaml-cpp)
target_link_libraries(${PROJECT_NAME} mariadbclientpp::mariadbclientpp)
