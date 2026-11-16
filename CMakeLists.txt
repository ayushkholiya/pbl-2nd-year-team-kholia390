cmake_minimum_required(VERSION 3.10)

project(uniroute LANGUAGES CXX)

add_executable(uniroute
    src/main.cpp
    src/common.cpp
)

target_include_directories(uniroute PRIVATE include)
target_compile_features(uniroute PRIVATE cxx_std_11)

if(MSVC)
    target_compile_options(uniroute PRIVATE /W4)
else()
    target_compile_options(uniroute PRIVATE -Wall -Wextra -Wpedantic)
endif()
