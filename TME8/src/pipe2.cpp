#include "pipe.h"

// Ecraser le fichier avec votre pipe.cpp pour la deuxième partie
namespace pr {

// Create the shm segment, initialize memory and semaphores.
int pipe_create(const char *name) { return -1;}

// Open an existing pipe : oflags is O_RDONLY or O_WRONLY
Pipe * pipe_open(const char *name, int oflags) {return nullptr;}

// Read up to count bytes from the pipe into buf
// Blocks if no data available
// Returns number of bytes read, 0 on EOF (no writers), -1 on error
// Assumes count <= PIPE_BUF
ssize_t pipe_read(Pipe *handle, void *buf, size_t count) {return -1;}

// Write up to count bytes from buf into the pipe
// Blocks if not enough space available
// Returns number of bytes written, -1 on error, raises SIGPIPE if no readers
// Assumes count <= PIPE_BUF
ssize_t pipe_write(Pipe *handle, const void *buf, size_t count) {return -1;}

// Close the pipe handle
// Decrements reader/writer count and unmaps memory
// Returns 0 on success, -1 and set errno on error
int pipe_close(Pipe *handle) { return -1;}

// Unlink (destroy) the pipe shared memory and semaphores
// Returns 0 on success, -1 and set errno on error
int pipe_unlink(const char *name){ return -1;}

}
