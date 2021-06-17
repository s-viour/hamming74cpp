cmake_minimum_required(VERSION 3.10)

project(
  hamming74cpp
  VERSION 0.1.0
  LANGUAGES CXX
)

add_library(hamming74cpp src/hamming74.cpp)
add_executable(hamming74 main.cpp)

target_include_directories(hamming74cpp PUBLIC include/)
target_link_libraries(hamming74 hamming74cpp)