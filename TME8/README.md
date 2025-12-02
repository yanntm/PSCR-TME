# TME8

This project builds two programs: `test_pipe`, `test_pipe2`.

## Dependencies

Posix system, non OSX (due to use of sem_t in shared memory).

## Build

1. Configure with CMake (Debug by default):
   ```
   mkdir build
   cd build
   cmake ..
   ```
   For Release build: `cmake -DCMAKE_BUILD_TYPE=Release ..`

2. Build:
   ```
   make
   ```

## Run

From the build directory: run test_pipe.
