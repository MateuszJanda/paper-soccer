# Useful link:
# https://raymii.org/s/tutorials/Cpp_project_setup_with_cmake_and_unit_tests.html

cmake_minimum_required(VERSION 3.6)

project(paper-soccer LANGUAGES CXX)

# exe placement in root directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-conversion")

add_compile_definitions(GAME_VERSION="1.0.0")


# Add directory to those the compiler uses to search for include files
include_directories(src)

# Compile source
add_subdirectory(src)

# Compile tests and googletest (excluded from ALL, so 'make runTests' must be called)
add_subdirectory(lib/googletest EXCLUDE_FROM_ALL)
add_subdirectory(tests EXCLUDE_FROM_ALL)
