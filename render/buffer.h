#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include "../common.h"

class vertexArray
{
    uint obj;
public:
    vertexArray();
    ~vertexArray();

    void gen();
    void free();

    void bind();
    void unbind();
};

class vertexBuffer
{
    uint obj, mode;
public:
    vertexBuffer();
    ~vertexBuffer();

    void gen();
    void free();

    void bind(const int = 0x8892); // GL_ARRAY_BUFFER
    void unbind();
};

void deleteArrays();

#endif // BUFFER_H_INCLUDED
