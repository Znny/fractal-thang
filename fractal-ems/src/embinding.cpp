#include <emscripten/bind.h>
#include "../include/gen.h"

EMSCRIPTEN_BINDINGS(my_module) {
    // Allow raw pointers for string constants
    //emscripten::allow_raw_pointers();
    
    //Constants
    //emscripten::constant("HELLO_WORLD", emscripten::allow_raw_pointer<const char*>(HELLO_WORLD));
    
    //Classes
    emscripten::class_<MyStruct>("MyStruct")
        .constructor<int, int>()
        .property("x", &MyStruct::x)
        .property("y", &MyStruct::y);
        
    //Functions
    emscripten::function("GenConstStruct", &GenConstStruct);
    emscripten::function("GenStruct", &GenStruct);
}










