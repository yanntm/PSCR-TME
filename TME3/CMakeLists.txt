cmake_minimum_required(VERSION 3.10)

# Project name and language.
project(countword LANGUAGES CXX)

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
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -pedantic -DNDEBUG -flto=auto")

# Add the executable (only main.cpp exists for now).
add_executable(countword
    src/main.cpp
    src/FileUtils.cpp
)

# Specify include directories.
target_include_directories(countword PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


find_package(Threads REQUIRED)
target_link_libraries(countword PRIVATE Threads::Threads)

# Programme autonome : aucune ABI publique ne dépend de cet alignement.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(countword PRIVATE -Wno-interference-size)
endif()
