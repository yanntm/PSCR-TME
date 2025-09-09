// String.h
#pragma once

#include <iostream> // For operator<< and traces
#include "strutil.h" // Assumes strutil.h is in namespace pr

// predeclare test class for friend access
class TestString;

namespace pr {

class String {
private:
    const char* data;

public:
    String(const char* s = ""); // Default and from C-string

    ~String();

// Uncomment as implemented:
//    String(const String& other); // Copy ctor
//
//    String& operator=(const String& other); // Copy assign
//
//    String(String&& other) noexcept; // Move ctor
//
//    String& operator=(String&& other) noexcept; // Move assign
//
//    bool operator<(const String& other) const; // Member for ordering
//
//    // Friends
//    friend std::ostream& operator<<(std::ostream& os, const String& str);
//    friend bool operator==(const String& a, const String& b); // Symmetric equality
//    friend String operator+(const String& a, const String& b); // Symmetric concat

    friend class ::TestString; // For private access in tests
};

} // namespace pr
