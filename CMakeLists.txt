cmake_minimum_required(VERSION 3.20)
project(MaanCache VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(MaanCache
    src/main.cpp
    src/database.cpp
    src/command.cpp
)

target_include_directories(MaanCache PUBLIC include)
