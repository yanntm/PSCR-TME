#include "pipe.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    const char *pipe_name = "test_pipe";
    
    std::cout << "Creating pipe..." << std::endl;
    
    // Create the pipe
    if (pr::pipe_create(pipe_name) == -1) {
        perror("pipe_create");
        return 1;
    }
    
    std::cout << "Opening read and write ends..." << std::endl;
    
    // Open both extremities before fork
    pr::Pipe *read_end = pr::pipe_open(pipe_name, O_RDONLY);
    pr::Pipe *write_end = pr::pipe_open(pipe_name, O_WRONLY);
    
    if (!read_end || !write_end) {
        perror("pipe_open");
        pr::pipe_unlink(pipe_name);
        return 1;
    }
    
    std::cout << "Forking..." << std::endl;
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        pr::pipe_close(read_end);
        pr::pipe_close(write_end);
        pr::pipe_unlink(pipe_name);
        return 1;
    }
    
    if (pid == 0) {
        std::cout << "Child: reading from pipe..." << std::endl;
        
        // Read and echo to stdout until EOF
        char buffer[256];
        ssize_t n;
        
        while ((n = pr::pipe_read(read_end, buffer, sizeof(buffer))) > 0) {
            std::cout << "Child: read " << n << " bytes" << std::endl;
            write(STDOUT_FILENO, buffer, n);
        }
        
        std::cout << "Child: EOF, closing read end..." << std::endl;
        
        // Close read end before exit
        pr::pipe_close(read_end);
        return 0;
    } else {
        // Parent process - only closes its write end
        std::cout << "Parent: writing to pipe..." << std::endl;
        
        // Write some data (parent still has read_end open, doesn't close it)
        const char *message = "Hello from parent process!\n";
        ssize_t written = pr::pipe_write(write_end, message, strlen(message));
        
        if (written == -1) {
            perror("pipe_write");
        } else {
            std::cout << "Parent: wrote " << written << " bytes" << std::endl;
        }
        
        std::cout << "Parent: closing write end..." << std::endl;
        
        // Close write end
        pr::pipe_close(write_end);
        
        // Wait for child
        int status;
        waitpid(pid, &status, 0);
        
        std::cout << "Parent: closing read end..." << std::endl;
        
        // Unlink the pipe
        pr::pipe_unlink(pipe_name);
        
        std::cout << "Parent done, child exited with status " << WEXITSTATUS(status) << std::endl;
    }
    
    return 0;
}
