cmake_minimum_required(VERSION 3.19)

project(pen_wheel VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(.)

add_subdirectory(pen_wheel)

include_directories(lib/json/include)
add_subdirectory(lib/json)

include_directories(lib/cereal/include)
add_subdirectory(lib/cereal)
