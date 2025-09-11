cmake_minimum_required(VERSION 3.10)

# Project name and language.
project(TME2 LANGUAGES CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a default build type if none is specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Set compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -DNDEBUG -flto")

# Add the executable (only main.cpp exists for now).
add_executable(TME2
    src/main.cpp
)

# Specify include directories.
target_include_directories(TME2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
