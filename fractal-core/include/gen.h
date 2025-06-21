#ifndef GEN_H
#define GEN_H

#include <string>

//extern const char* HELLO_WORLD;

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
