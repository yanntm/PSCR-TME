#include <iostream>
#include <cassert>
#include <cstring> // For initial comparisons if needed
#include <utility> // For std::rel_ops
#include "String.h"
#include "strutil.h"

using namespace pr; // For simplicity in tests

class TestString {
public:
    static void test_assert(bool condition, const std::string& message) {
        if (!condition) {
            std::cerr << "Test failed: " << message << std::endl;
        } else {
            std::cout << "Test passed: " << message << std::endl;
        }
    }

    // Accessors for private data (since friend)
    static const char* getData(const String& s) {
        return s.data;
    }

    static bool dataPointersDiffer(const String& a, const String& b) {
        return getData(a) != getData(b);
    }

    static bool contentsMatch(const String& a, const String& b) {
        return compare(getData(a), getData(b)) == 0;
    }

    // Helper function to create a temporary String (for natural rvalue tests)
    static String makeString(const char* content) {
        String temp(content);
        // Add some logic to potentially disable NRVO (compiler-dependent)
        if (length(content) > 0) {
            return temp;
        }
        return String("");
    }

    // Individual test functions
    static void testLength() {
        std::cout << "\n--- Testing length ---" << std::endl;
        test_assert(length("Hello") == 5, "length of 'Hello' should be 5");
        test_assert(length("") == 0, "length of empty string should be 0");
        test_assert(length("a") == 1, "length of 'a' should be 1");
    }

    static void testNewcopy() {
        std::cout << "\n--- Testing newcopy ---" << std::endl;
        const char* test_str = "Hello";
        char* copy = newcopy(test_str);
        test_assert(copy != test_str, "newcopy should return a different pointer");
        test_assert(std::strcmp(copy, test_str) == 0, "newcopy contents should match original");
        delete[] copy;

        const char* empty_str = "";
        char* empty_copy = newcopy(empty_str);
        test_assert(empty_copy != empty_str, "newcopy of empty should return different pointer");
        test_assert(std::strcmp(empty_copy, empty_str) == 0, "newcopy of empty contents should match");
        delete[] empty_copy;
    }

    /*
    static void testCompare() {
        std::cout << "\n--- Testing compare ---" << std::endl;
        test_assert(compare("abc", "abc") == 0, "compare equal strings");
        test_assert(compare("abc", "abd") < 0, "compare abc < abd");
        test_assert(compare("abd", "abc") > 0, "compare abd > abc");
        test_assert(compare("ab", "abc") < 0, "compare shorter first");
        test_assert(compare("abc", "ab") > 0, "compare longer first");
        test_assert(compare("", "") == 0, "compare empty strings");
        test_assert(compare("", "a") < 0, "empty < non-empty");
        test_assert(compare("a", "") > 0, "non-empty > empty");
    }
    */

    /*
    static void testConstructorAndOutput() {
        std::cout << "\n--- Testing String constructor and output ---" << std::endl;
        String s1("Hello, World!");
        std::cout << "s1: " << s1 << std::endl; // Visual check
        test_assert(contentsMatch(s1, String("Hello, World!")), "constructor contents match");
        // Destructor tested with valgrind and traces
    }
    */

    /*
    static void testCopyConstructor() {
        std::cout << "\n--- Testing copy constructor ---" << std::endl;
        String s1("Copy Test");
        String s2(s1);
        test_assert(&s2 != &s1, "copy ctor creates new object");
        test_assert(dataPointersDiffer(s1, s2), "copy ctor data pointers differ");
        test_assert(contentsMatch(s1, s2), "copy ctor contents match");
        std::cout << "s2 (copy of s1): " << s2 << std::endl; // Visual check
    }
    */

    /*
    static void testAssignmentOperator() {
        std::cout << "\n--- Testing assignment operator ---" << std::endl;
        String s1("Assign Test");
        String s3;
        s3 = s1;
        test_assert(dataPointersDiffer(s1, s3), "assignment data pointers differ");
        test_assert(contentsMatch(s1, s3), "assignment contents match");
        std::cout << "s3 (assigned from s1): " << s3 << std::endl; // Visual check

        // Self-assignment
        String s4("Self Assign");
        const char* old_data = getData(s4);
        s4 = s4;
        test_assert(getData(s4) == old_data, "self-assignment keeps data pointer");
        test_assert(contentsMatch(s4, String("Self Assign")), "self-assignment contents unchanged");
    }
    */

    /*
    static void testOperatorEqual() {
        std::cout << "\n--- Testing operator== ---" << std::endl;
        String s1("Equal");
        String s2("Equal");
        String s4("Different");
        test_assert(s1 == s2, "equal strings == true");
        test_assert(!(s1 == s4), "different strings == false");
        String empty1("");
        String empty2("");
        test_assert(empty1 == empty2, "empty strings == true");

        // Test symmetry with const char*
        test_assert(s1 == "Equal", "String == const char*");
        test_assert("Equal" == s1, "const char* == String");
    }
    */

    /*
    static void testOperatorLess() {
        std::cout << "\n--- Testing operator< ---" << std::endl;
        String s5("Apple");
        String s6("Banana");
        test_assert(s5 < s6, "Apple < Banana");
        test_assert(!(s6 < s5), "not Banana < Apple");
        test_assert(!(s5 < s5), "not Apple < Apple");
        String empty("");
        test_assert(empty < s5, "empty < Apple");
        test_assert(!(s5 < empty), "not Apple < empty");
    }
    */

    /*
    static void testRelOps() {
        std::cout << "\n--- Testing std::rel_ops generated operators ---" << std::endl;
        {
            using namespace std::rel_ops; // Scoped to this block
            String s5("Apple");
            String s6("Banana");
            test_assert(s6 > s5, "Banana > Apple via rel_ops");
            test_assert(s5 <= s6, "Apple <= Banana via rel_ops");
            test_assert(s6 >= s5, "Banana >= Apple via rel_ops");
            test_assert(s5 != s6, "Apple != Banana via rel_ops");
            test_assert(!(s5 != s5), "not Apple != Apple via rel_ops");
        }
    }
    */

    /*
    static void testNewcat() {
        std::cout << "\n--- Testing newcat ---" << std::endl;
        const char* a = "Hello";
        const char* b = " World";
        char* cat = newcat(a, b);
        test_assert(std::strcmp(cat, "Hello World") == 0, "newcat contents match");
        test_assert(cat != a && cat != b, "newcat new pointer");
        delete[] cat;

        char* empty_cat = newcat("", "Test");
        test_assert(std::strcmp(empty_cat, "Test") == 0, "empty + str");
        delete[] empty_cat;
    }
    */

    /*
    static void testOperatorPlus() {
        std::cout << "\n--- Testing operator+ ---" << std::endl;
        String s7("Hello");
        String s8(" World");
        String result = s7 + s8;
        test_assert(contentsMatch(result, String("Hello World")), "concat contents match");
        test_assert(dataPointersDiffer(s7, result), "concat creates new data");
        test_assert(dataPointersDiffer(s8, result), "concat creates new data");
        std::cout << "result: " << result << std::endl; // Visual check

        // Test with empty
        String empty("");
        test_assert(empty + s7 == s7, "empty + String == String");
        test_assert(s7 + empty == s7, "String + empty == String");

        // Natural rvalue from + (observe traces for copies before moves are added)
        String combined = s7 + s8 + String("!"); // Chains + , may involve temporaries
    }
    */

    /*
    static void testMoveConstructor() {
        std::cout << "\n--- Testing move constructor (observe traces for moves) ---" << std::endl;
        // Natural rvalue from function return
        String s5 = makeString("Move Test");
        test_assert(contentsMatch(s5, String("Move Test")), "move ctor from return contents match");
        // If moves implemented, traces should show move ctor; else copy

        // From prvalue temporary
        String s6 = String("Prvalue Test");
        test_assert(contentsMatch(s6, String("Prvalue Test")), "move ctor from prvalue contents match");
    }
    */

    /*
    static void testMoveAssignment() {
        std::cout << "\n--- Testing move assignment (observe traces for moves) ---" << std::endl;
        String s7;
        // Assign from natural rvalue (function return)
        s7 = makeString("Move Assign Test");
        test_assert(contentsMatch(s7, String("Move Assign Test")), "move assign from return contents match");

        // Assign from temporary
        String s8;
        s8 = String("Temp Assign");
        test_assert(contentsMatch(s8, String("Temp Assign")), "move assign from temporary contents match");
    }
    */

    static void runAllTests() {
        testLength();
        testNewcopy();
        // testCompare();
        // testConstructorAndOutput();
        // testCopyConstructor();
        // testAssignmentOperator();
        // testOperatorEqual();
        // testOperatorLess();
        // testRelOps();
        // testNewcat();
        // testOperatorPlus();
        // testMoveConstructor();
        // testMoveAssignment();

        std::cout << "\nAll uncommented tests completed." << std::endl;
    }
};

int main() {
    TestString::runAllTests();

    // Run under valgrind: valgrind --leak-check=full ./test_string
    // Should report no leaks or errors

    return 0;
}
