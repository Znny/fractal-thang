// TEMPORARY/GARBAGE CODE - Will be removed when actual fractal development begins
// This is just placeholder code for testing the build system

//std includes
#include <string>
#include <iostream>

//local includes
#include "../include/gen.h"

MyStruct::MyStruct(int x, int y) : x(x), y(y) {}

MyStruct GenConstStruct() {
    return MyStruct(1, 2);
}

MyStruct GenStruct(int x, int y) {
    return MyStruct(x, y);
}

std::string getHelloWorld() {
    return "Hello, World! wooh";
}

std::string getMessage(int x) {
    return "Result from C++: " + std::to_string(x);
} 