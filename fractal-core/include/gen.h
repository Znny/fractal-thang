#ifndef GEN_H
#define GEN_H

#include <string>

// TEMPORARY/GARBAGE CODE - Will be removed when actual fractal development begins
// This is just placeholder code for testing the build system

struct MyStruct {
    int x;
    int y;
    MyStruct(int x, int y);
};

MyStruct GenConstStruct();
MyStruct GenStruct(int x, int y);

std::string getHelloWorld();
std::string getMessage(int x);

#endif // GEN_H
