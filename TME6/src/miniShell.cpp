#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>

// Custom mystrdup : allocates with new[], copies string (avoid strdup and free)
char* mystrdup(const char* src);

int main() {
    std::string line;

    while (true) {
        std::cout << "mini-shell> " << std::flush;
        if (!std::getline(std::cin, line)) {
            std::cout << "\nExiting on EOF (Ctrl-D)." << std::endl;
            break;
        }

        if (line.empty()) continue;

        // Simple parsing: split by spaces using istringstream
        std::istringstream iss(line);
        std::vector<std::string> args;
        std::string token;
        while (iss >> token) {
            args.push_back(token);
        }
        if (args.empty()) continue;

        // Prepare C-style argv: allocate and mystrdup
        char** argv = new char*[args.size()];
        for (size_t i = 0; i < args.size(); ++i) {
            argv[i] = mystrdup(args[i].c_str());
        }

        // Your code for fork/exec/wait/signals goes here...

        // cleanup argv allocations
        for (size_t i = 0; i < args.size(); ++i) {
            delete[] argv[i];
        }
        delete[] argv;
    }
    return 0;
}



char* mystrdup(const char* src) {
    if (src == nullptr) return nullptr;
    size_t len = strlen(src) + 1;  // +1 for null terminator
    char* dest = new char[len];
    memcpy(dest, src, len);  // Or strcpy if preferred
    return dest;
}
