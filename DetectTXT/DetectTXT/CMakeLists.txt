cmake_minimum_required(VERSION 3.5)

project(DetectTXT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(DetectTXT main.cpp)
target_link_libraries(DetectTXT ${OpenCV_LIBS} )


install(TARGETS DetectTXT
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
