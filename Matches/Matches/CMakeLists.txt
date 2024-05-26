cmake_minimum_required(VERSION 3.5)

project(Matches LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(Matches main.cpp)
target_link_libraries( Matches ${OpenCV_LIBS} )

install(TARGETS Matches
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
