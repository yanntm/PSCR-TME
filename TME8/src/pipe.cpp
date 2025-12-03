#include "pipe.h"

#include <fcntl.h>
#include <unistd.h>
#include <limits.h> // For PIPE_BUF
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <signal.h>
#include <algorithm>

namespace pr {

// Place this in shared memory !    
struct PipeShm {
    char buffer[PIPE_BUF];
    size_t head; // write position
    size_t tail; // read position
    size_t count; // number of bytes in the pipe
    size_t nbReaders; // number of readers
    size_t nbWriters; // number of writers
    // TODO : semaphores
};

// This is per-process handle, not in shared memory
struct Pipe {
    PipeShm *shm; // pointer to shared memory
    int oflags; // O_RDONLY or O_WRONLY
};

int pipe_create(const char *name) {
    // Construct shared memory name
    char shm_name[256];
    // add a '/' at the beginning for shm_open
    snprintf(shm_name, sizeof(shm_name), "/%s", name);
    
    // Try to create shared memory with O_CREAT|O_EXCL
    // Set size of shared memory
    // Map the shared memory 
    // Initialize the PipeShm structure

    // Including semaphores

    // Unmap and close (setup persists in shared memory)
    
    return 0;
}

Pipe * pipe_open(const char *name, int oflags) {
    // Construct shared memory name
    char shm_name[256];
    snprintf(shm_name, sizeof(shm_name), "/%s", name);
    
    // Open shared memory (without O_CREAT)
    // Map the shared memory
    // Can close fd after mmap
    // Increment nbReaders or nbWriters
    
    // Create and return Pipe handle
    Pipe *handle = nullptr; // new Pipe();
    // handle->shm = ...
    return handle;
}

ssize_t pipe_read(Pipe *handle, void *buf, size_t count) {
    if (handle == nullptr || handle->oflags != O_RDONLY) {
        errno = EBADF;
        return -1;
    } else if (count > PIPE_BUF) {
        errno = EINVAL;
        return -1;
    }
    
    // wait until some data available or no writers

    // Check if pipe is empty and no writers : EOF
    
    // Read min(count, shm->count) bytes
    PipeShm *shm = handle->shm;
    size_t to_read = std::min(count, shm->count);
    char *output = (char *)buf;
    
    // Handle circular buffer: may need to copy in two parts
    size_t first_chunk = std::min(to_read, PIPE_BUF - shm->tail);
    memcpy(output, &shm->buffer[shm->tail], first_chunk);
    
    if (first_chunk < to_read) {
        // Wrap around to beginning of buffer
        memcpy(output + first_chunk, &shm->buffer[0], to_read - first_chunk);
    }
    
    shm->tail = (shm->tail + to_read) % PIPE_BUF;
    shm->count -= to_read;
    
    // warn other readers/writers if needed
    
    return to_read;
}

ssize_t pipe_write(Pipe *handle, const void *buf, size_t count) {
    if (handle == nullptr || handle->oflags != O_WRONLY) {
        errno = EBADF;
        return -1;
    } else if (count > PIPE_BUF) {
        errno = EINVAL;
        return -1;
    }
    
    PipeShm *shm = handle->shm;
    
    // wait until *enough* space available or no readers
    
    // Check if no readers => SIGPIPE
    
    // Write count bytes
    const char *input = (const char *)buf;
    
    // Handle circular buffer: may need to copy in two parts
    size_t first_chunk = std::min(count, PIPE_BUF - shm->head);
    memcpy(&shm->buffer[shm->head], input, first_chunk);
    
    if (first_chunk < count) {
        // Wrap around to beginning of buffer
        memcpy(&shm->buffer[0], input + first_chunk, count - first_chunk);
    }
    
    shm->head = (shm->head + count) % PIPE_BUF;
    shm->count += count;
    
    // warn other readers/writers if needed
    
    return count;
}

int pipe_close(Pipe *handle) {
    if (handle == nullptr) {
        errno = EBADF;
        return -1;
    }
    
    PipeShm *shm = handle->shm;
    
    // Decrement reader or writer count
    // Warn other process as needed (e.g. if last reader/writer)
    
    // Unmap memory
    // Free handle
    delete handle;
    
    return 0;
}

int pipe_unlink(const char *name) {
    // Construct shared memory name
    char shm_name[256];
    snprintf(shm_name, sizeof(shm_name), "/%s", name);
    
    // one last shm_open + mmap : we need to sem_destroy

    // Unlink shared memory
    
    return 0;
}

} // namespace pr

