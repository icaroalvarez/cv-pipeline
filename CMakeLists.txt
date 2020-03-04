cmake_minimum_required(VERSION 3.5)

project(Panoramix)
set(CMAKE_CXX_STANDARD 17)
message("CMake build type: " ${CMAKE_BUILD_TYPE})

include(.conan/conanbuildinfo.cmake)
get_directory_property(hasParent PARENT_DIRECTORY)
if(!hasParent)
    conan_basic_setup(TARGETS)
endif()

find_package(Threads)

add_subdirectory(src)
add_subdirectory(test)