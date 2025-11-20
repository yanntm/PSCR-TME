#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char **argv) {

  // Parse command line arguments: find the pipe separator "|"
  // Format: ./pipe cmd1 [args...] | cmd2 [args...]

  // Create a pipe for inter-process communication

  // Fork the first child process ; child redirects out to write end of pipe, then exec

  // Fork the second child process ; child redirects in from read end of pipe, then exec

  // Wait for both children to finish

  return 0;
}
