# CMake minimum version required to build this project.
cmake_minimum_required(VERSION 3.10)

# Project name and language.
project(TME1 LANGUAGES CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Avoid compiler-specific extensions.

# Set a default build type if none is specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -DNDEBUG -flto")

# Add the executable.
add_executable(TestList
    src/main.cpp
    src/List.cpp
    src/List.h
)

# Specify include directories for clarity.
target_include_directories(TestList PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)