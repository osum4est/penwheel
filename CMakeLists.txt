cmake_minimum_required(VERSION 3.19)

project(pen_wheel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(.)

add_subdirectory(lib/yaml-cpp)

add_subdirectory(lib/cereal)
include_directories(lib/cereal/include)

add_subdirectory(pen_wheel)
