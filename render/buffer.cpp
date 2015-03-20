#include <GL/glew.h>
#include <list>
#include "buffer.h"

std::list<vertexArray*> VAOlist;
std::list<vertexBuffer*> VBOlist;

vertexBuffer *oldBuffer = 0;
vertexArray *oldArray = 0;

vertexArray::vertexArray()
{
    obj = 0;
}

vertexArray::~vertexArray()
{
    if(obj)
        free();
}

void vertexArray::gen()
{
    if(glIsVertexArray(obj))
        return;
    glGenVertexArrays(1, &obj);
    VAOlist.push_back(this);
}

void vertexArray::free()
{
    if(!glIsVertexArray(obj))
        return;
    unbind();
    glDeleteVertexArrays(1, &obj);
    obj = 0;
    VAOlist.remove(this);
}

void vertexArray::bind()
{
    if(obj && oldArray != this)
    {
        glBindVertexArray(obj);
        oldArray = this;
    }

}

void vertexArray::unbind()
{
    glBindVertexArray(0);
    oldArray = 0;
}

vertexBuffer::vertexBuffer()
{
    obj = 0;
}

vertexBuffer::~vertexBuffer()
{
    if(obj)
        free();
}

void vertexBuffer::gen()
{
    if(glIsBuffer(obj))
        return;
    glGenBuffers(1, &obj);
    VBOlist.push_back(this);
}

void vertexBuffer::free()
{
    if(!glIsBuffer(obj))
        return;
    unbind();
    glDeleteBuffers(1, &obj);
    obj = 0;
    VBOlist.remove(this);
    oldBuffer = 0;
}

void vertexBuffer::bind(const int m)
{
    mode = m;
    if(obj && oldBuffer != this)
    {
        glBindBuffer(mode, obj);
        oldBuffer = this;
    }

}

void vertexBuffer::unbind()
{
    glBindBuffer(mode, 0);
    oldBuffer = 0;
}

void deleteArrays()
{
    for(auto i:VAOlist)
        i->free();
    for(auto i:VBOlist)
        i->free();

    VAOlist.clear();
    VBOlist.clear();
}
