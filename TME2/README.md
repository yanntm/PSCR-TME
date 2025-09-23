# TME2

This document explains how to build and run the `TME2` program.

The build system uses CMake, which generates native build files (like Makefiles on Linux) from a `CMakeLists.txt` configuration file.

## Build Configurations

This project is set up with two main build configurations:

-   **Debug**: This is the default configuration. It compiles the code with no optimization (`-O0`) and includes debugging symbols (`-g`). This version is larger and slower, but it allows you to use a debugger (like `gdb`) to step through the code and inspect variables. It also enables extensive warnings (`-Wall -Wextra -pedantic`) to help catch potential errors.

-   **Release**: This configuration is optimized for performance. It uses a high level of optimization (`-O3`) and removes debugging symbols. The resulting executable is smaller and faster. This is the version you would typically distribute.

## How to Build

It is recommended to use a separate directory for each build configuration to avoid conflicts.

### Debug Build (Default)

1.  **Create and enter a build directory:**
    ```
    mkdir build-debug
    cd build-debug
    ```

2.  **Run CMake.** It will default to the Debug configuration.
    ```
    cmake ..
    ```

3.  **Run Make.** This compiles the `TME2` executable.
    ```
    make
    ```

### Release Build

1.  **Create and enter a build directory:**
    ```
    mkdir build-release
    cd build-release
    ```

2.  **Run CMake, specifying the Release build type:**
    ```
    cmake -DCMAKE_BUILD_TYPE=Release ..
    ```

3.  **Run Make.** This compiles the optimized `TME2` executable.
    ```
    make
    ```

## How to Run

The executable `TME2` will be created inside the build directory you chose (`build-debug` or `build-release`).

To run it from within the build directory:
```
./TME2 [path/to/textfile] [mode]
```

If no filename is given the program will try to read `../WarAndPeace.txt` (one level above the build directory).

If no mode is provided the program will use mode "count".