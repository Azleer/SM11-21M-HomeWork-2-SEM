cmake_minimum_required(VERSION 3.5)

project(DetectTXT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(DetectTXT main.cpp)

install(TARGETS DetectTXT
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
