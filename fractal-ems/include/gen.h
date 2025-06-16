#ifndef GEN_H
#define GEN_H

//extern const char* HELLO_WORLD;

struct MyStruct {
    int x;
    int y;
    MyStruct(int x, int y);
};

MyStruct GenConstStruct();
MyStruct GenStruct(int x, int y);

#endif // GEN_H
