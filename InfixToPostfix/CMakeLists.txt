cmake_minimum_required(VERSION 3.5.1)

project(InfixToPostfix VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")

add_executable(infix_to_postfix infix_to_postfix.cpp)