cmake_minimum_required(VERSION 3.5)

project(Slodu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Example1 main.cpp)
add_executable(Example2 main2.cpp)
add_executable(SysEqual-RK4 RK4.cpp)

install(TARGETS SysEqual-RK4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
