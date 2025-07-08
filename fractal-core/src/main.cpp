//std includes
#include <iostream>
#include "gen.h"


int main(int argc, char** argv) {
    std::cout << "Hello, World, from main.cpp" << std::endl;
    std::cout << getHelloWorld() << std::endl;
    std::cout << getMessage(42) << std::endl;
    MyStruct testStruct = GenStruct(10, 20);
    std::cout << "x: " << testStruct.x << ", y: " << testStruct.y << std::endl;
    return 0;
}