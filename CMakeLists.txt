cmake_minimum_required(VERSION 3.16)
project(chess-engine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(chessEngine INTERFACE)
add_library(chess-engine::chessEngine ALIAS chessEngine)
target_compile_features(chessEngine INTERFACE cxx_std_17)
target_include_directories(chessEngine INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

include(GNUInstallDirs)
install(TARGETS chessEngine EXPORT chess-engine-targets)
install(FILES src/chessEngine.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT chess-engine-targets
    FILE chess-engine-config.cmake
    NAMESPACE chess-engine::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/chess-engine)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_executable(chess-engine src/main.cpp)
    target_link_libraries(chess-engine PRIVATE chessEngine)

    enable_testing()
    add_executable(chess-tests tests/tests.cpp)
    target_link_libraries(chess-tests PRIVATE chessEngine)
    add_test(NAME tests COMMAND chess-tests)
endif()
