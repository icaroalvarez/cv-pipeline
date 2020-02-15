cmake_minimum_required(VERSION 3.5)

project(Panoramix)
set(CMAKE_CXX_STANDARD 17)
message("CMake build type: " ${CMAKE_BUILD_TYPE})

include(.conan/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
message(${CONAN_INCLUDE_DIRS})

find_package(OpenCV REQUIRED COMPONENTS core highgui)
find_package(Threads)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(gui_qt)