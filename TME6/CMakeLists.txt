cmake_minimum_required(VERSION 3.16)
project(TME6 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type to Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

include(CheckIncludeFileCXX)

include_directories(src/)

check_include_file_cxx(unistd.h HAVE_UNISTD_H)

if(HAVE_UNISTD_H)
  message(STATUS "POSIX header unistd.h found")
else()
  message(FATAL_ERROR "POSIX header unistd.h not found; this may not be a POSIX system")
endif()

# Set compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")

# Define executables
add_executable(combat src/combat.cpp src/util/rsleep.cpp src/util/mtrand.cpp)
add_executable(miniShell src/miniShell.cpp)