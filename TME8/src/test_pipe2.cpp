#include "pipe.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    const char *pipe_name = "test_pipe2";
    
    // 1. Main creates the pipe, no open yet
    if (pr::pipe_create(pipe_name) == -1) {
        perror("pipe_create");
        return 1;
    }
    
    // 2. Fork a child
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    
    if (pid == 0) {
        // Child: opens read end (will block until parent opens write end)
        std::cout << "Child: opening read end (blocking until writer)..." << std::endl;
        pr::Pipe *read_end = pr::pipe_open(pipe_name, O_RDONLY);
        
        if (!read_end) {
            perror("pipe_open read");
            return 1;
        }
        
        std::cout << "Child: read end opened, reading..." << std::endl;
        
        // Read until EOF
        char buffer[256];
        ssize_t n;
        
        while ((n = pr::pipe_read(read_end, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, n);
        }
        
        std::cout << "Child: EOF, closing..." << std::endl;
        pr::pipe_close(read_end);
        return 0;
    } else {
        // Parent: sleep a bit to show blocking behavior, then open write end
        std::cout << "Parent: sleeping 1 second before opening write end..." << std::endl;
        sleep(1);
        
        std::cout << "Parent: opening write end..." << std::endl;
        pr::Pipe *write_end = pr::pipe_open(pipe_name, O_WRONLY);
        
        if (!write_end) {
            perror("pipe_open write");
            return 1;
        }
        
        std::cout << "Parent: write end opened, writing..." << std::endl;
        
        // Write message
        const char *message = "Hello from parent via blocking pipe!\n";
        ssize_t written = pr::pipe_write(write_end, message, strlen(message));
        
        if (written == -1) {
            perror("pipe_write");
        }
        
        std::cout << "Parent: closing write end..." << std::endl;
        pr::pipe_close(write_end);
        
        std::cout << "Parent: closed write end..." << std::endl;
        // Wait for child
        int status;
        waitpid(pid, &status, 0);
        
        // Unlink
        pr::pipe_unlink(pipe_name);
        
        std::cout << "Parent: done" << std::endl;
    }
    
    return 0;
}
