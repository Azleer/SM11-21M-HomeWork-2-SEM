cmake_minimum_required(VERSION 3.5)

project(D-Prince LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN_INCLUDE_DIR})

add_executable(D-Prince main.cpp)

target_link_libraries(D-Prince Eigen3::Eigen)

install(TARGETS D-Prince
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
