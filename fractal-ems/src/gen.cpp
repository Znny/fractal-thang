//emscripten includes
#include <emscripten/bind.h>

//local includes
#include "../include/gen.h"

MyStruct::MyStruct(int x, int y) : x(x), y(y) {}

MyStruct GenConstStruct() {
    return MyStruct(1, 2);
}

MyStruct GenStruct(int x, int y) {
    return MyStruct(x, y);
}

