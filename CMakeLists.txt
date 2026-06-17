cmake_minimum_required(VERSION 3.15)
project(OrderBookPlatform LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(OrderBookExecutable 
    src/main.cpp 
    src/CsvParser.cpp
)