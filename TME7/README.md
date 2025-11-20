# TME7

This project builds three programs: `graphIO`, `pipe`, and `mycat`.

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

### graphIO
The `graphIO` program converts between text-based edge list format and binary graph format. It takes three arguments: mode (`c` for compress to binary, `d` for decompress to text), input file, and output file. Example graph files are provided in the `examples/` directory.

Example usage:
- Compress an edge list to binary: `./graphIO c examples/facebook_combined.txt output.bin`
- Decompress a binary file to edge list: `./graphIO d output.bin output.txt`

### pipe
The `pipe` program simulates shell piping by chaining two commands, where the output of the first command feeds into the input of the second. It takes the commands as arguments separated by `|`.

Example usage:
- Chain `cat` and `wc`: `./pipe /bin/cat pipe.cpp \| /bin/wc -l`

### mycat
The `mycat` program is a simple implementation of the `cat` command. It reads from stdin if no arguments are provided, or concatenates the contents of the specified files to stdout. If a file does not exist, it prints an error to stderr and continues.

Example usage:
- Read from stdin: `echo "Hello World" | ./mycat`
- Concatenate files: `./mycat file1.txt file2.txt`