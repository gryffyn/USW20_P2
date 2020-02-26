cmake_minimum_required(VERSION 3.15)
project(USW20_P2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS --coverage)

add_executable(USW20_P2 main.cpp libs/Key.cpp libs/User.cpp libs/User.h libs/Student.cpp libs/Student.h)
find_package(Boost)
link_directories(libs/sodium)
include_directories(libs/sodium)
target_link_libraries(${PROJECT_NAME} sodium)