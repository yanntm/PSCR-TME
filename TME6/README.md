# TME6

This project builds two programs: `combat` and `miniShell`.

## Dependencies

None.

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

From the build directory:
- `./combat`: Runs the combat program.
- `./miniShell`: Runs the mini shell program.