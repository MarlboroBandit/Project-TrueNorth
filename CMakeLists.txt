cmake_minimum_required(VERSION 3.10)

project(Project-TrueNorth VERSION 1.0)

set(CMAKE_CXX_STANDARD 23) # I am using C++ version 23 but C++17 will do atleast.
set(CMAKE_CXX_STANDARD True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpendantic)
endif()

add_executable(blackjack_sim
  main.cpp 
  engine.cpp
  header.h
)
