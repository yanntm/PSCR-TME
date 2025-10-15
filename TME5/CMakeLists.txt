cmake_minimum_required(VERSION 3.10)

# Project name and language.
project(TME5 LANGUAGES CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a default build type if none is specified.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -DNDEBUG -flto")

# Add the executable (only main.cpp exists for now).
add_executable(TME5
    src/main.cpp
    src/util/mtrand.cpp
)

# Specify include directories.
target_include_directories(TME5 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link Qt libraries
target_link_libraries(TME5)

