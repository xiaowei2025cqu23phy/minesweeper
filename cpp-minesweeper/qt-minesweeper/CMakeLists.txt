cmake_minimum_required(VERSION 3.16)
project(qt_minesweeper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(qt_minesweeper main.cpp)
target_link_libraries(qt_minesweeper PRIVATE Qt6::Widgets)
