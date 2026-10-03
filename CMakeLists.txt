cmake_minimum_required(VERSION 3.15)
project(OrderBookPlatform LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(OrderBookExecutable
    src/main.cpp
    src/CsvParser.cpp
    src/BookReconstruction.cpp
)

target_include_directories(OrderBookExecutable PRIVATE include)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(OrderBookExecutable PRIVATE -Wall -Wextra -Wpedantic)
endif()