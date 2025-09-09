# TME1 - TestString Exercise

This exercise is designed to help you understand the fundamentals of memory management in C++ by building your own `String` class.

## Goal

The primary goal is to implement the `pr::String` class and its helper functions in `src/String.cpp` and `src/strutil.cpp`. You will do this by progressively uncommenting the test functions in `src/main.cpp` and implementing the required functionality until all tests pass without memory leaks.

## Build Instructions

To build the project, you can use the provided CMake configuration. From the root of the `TestString` directory, run the following commands:

```bash
mkdir build
cd build
cmake ..
make
```

Alternatively, if you are using VSCode with the CMake Tools extension, you can run the build tasks directly from the IDE.

## Progression

Follow this progression to complete the exercise. After implementing each piece of functionality, uncomment the corresponding test function in `main.cpp` to verify your implementation.

### 1. C-string Utilities (`strutil.cpp`)

Start by implementing the basic C-string utility functions in `src/strutil.cpp`.

- **`length`**: Implement the `length` function to calculate the length of a C-string.
- **`newcopy`**: Implement `newcopy` to create a new copy of a C-string.
- **`compare`**: Implement `compare` to compare two C-strings.

### 2. `String` Class Implementation (`String.cpp`)

Now, start implementing the `pr::String` class.

- **Constructor & Destructor**: Implement the constructor `String(const char* s)` and the destructor `~String()`. Remember to trace their execution by printing to `std::cout` as shown in the examples.
- **Copy Constructor**: Implement the copy constructor `String(const String& other)`.
- **Copy Assignment Operator**: Implement the copy assignment operator `operator=(const String& other)`.
- **`operator==` (friend)**: Implement the equality operator `operator==` as a friend function.
- **`operator<` (member)**: Implement the less-than operator `operator<` as a member function.
- **`operator+`**: Implement the concatenation operator `operator+`.

### 3. Advanced Features (Move Semantics)

Once the basic functionality is working, you can move on to implementing move semantics for better performance.

- **Move Constructor**: Implement the move constructor `String(String&& other)`.
- **Move Assignment Operator**: Implement the move assignment operator `operator=(String&& other)`.

## Running Tests with Valgrind

It is crucial to check for memory leaks throughout this exercise. You can use Valgrind to run the tests and detect memory issues.

If you are using VSCode, you can run the "Valgrind: Run Target (Debug)" task from the command palette.

Alternatively, you can run Valgrind from the command line:

```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/TestString
```

Good luck!
