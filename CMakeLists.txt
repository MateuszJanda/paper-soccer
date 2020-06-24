# Useful link:
# https://raymii.org/s/tutorials/Cpp_project_setup_with_cmake_and_unit_tests.html

cmake_minimum_required(VERSION 3.5)

project(paper-soccer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Download and install GoogleTest
set(GTEST_PROJECT googletest)
set(GTEST_PREFIX "${CMAKE_SOURCE_DIR}/lib")

include(ExternalProject)
ExternalProject_Add(${GTEST_PROJECT}
    GIT_REPOSITORY https://github.com/google/googletest.git
    PREFIX ${GTEST_PREFIX}
    CMAKE_ARGS ""
)
ExternalProject_Get_Property(${GTEST_PROJECT} source_dir binary_dir)

include_directories(src)

add_subdirectory(src)
# Add googletest sources
add_subdirectory(${source_dir})
add_subdirectory(tests)
