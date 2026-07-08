cmake_minimum_required(VERSION 3.16)
project(chess-engine VERSION 0.5.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chess-engine src/main.cpp)

enable_testing()
add_executable(chess-tests tests/tests.cpp)
add_test(NAME tests COMMAND chess-tests)
