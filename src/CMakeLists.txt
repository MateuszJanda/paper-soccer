add_executable(${CMAKE_PROJECT_NAME} main.cpp Board.cpp)
add_library(${CMAKE_PROJECT_NAME}_lib STATIC Board.cpp)
