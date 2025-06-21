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
    return "Hello, World!";
}

std::string getMessage(int x) {
    return "Result from C++: " + std::to_string(x);
}

int main(int argc, char** argv) {
    std::cout << "Hello, World, from main.cpp, hot reloading live" << std::endl;
    return 0;
}