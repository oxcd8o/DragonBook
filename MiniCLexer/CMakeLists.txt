cmake_minimum_required(VERSION 3.5.1)

project(MiniCLexer VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")

add_executable(mini_c_lexer mini_c_lexer.cpp)

target_include_directories(mini_c_lexer PRIVATE .)