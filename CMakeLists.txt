cmake_minimum_required(VERSION 3.16)

project(AyushDB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    AyushDB
    src/main.cpp
    src/database/database.cpp
)

target_include_directories(AyushDB PRIVATE include)
