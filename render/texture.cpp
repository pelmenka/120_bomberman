#include <GL/glew.h>
#include <string>
#include <math.h>
#include "../common.h"
#include "../texops.h"
#include "texture.h"

#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923

#include <list>

using std::list;

list<texture*> imgList;

const int drawImageBufSize = 128*4;

void flushTextures()
{
    for(auto i: imgList)
        i->flush();
}

void cleanTextures()
{
    for(auto i: imgList)
        i->unload();
}

texture::texture()
{
    size = {-1, -1};
    refCount = 1;
    color = {1, 1, 1, 1};
    scale = {1, 1};
    parts = {1, 1};
    data = nullptr;
    parent = nullptr;
    setAngle(0);
    tex = 0;
}

texture::texture(std::string s, int q)
{
    texture();
    load(s, q);
}

texture::~texture()
{
    unload(true);
}

void texture::_alloc()
{
    if(data == nullptr)
        data = new _imgVert[drawImageBufSize];

    imgList.push_back(this);
    drawCalls = 0;
}

bool texture::unload(bool end)
{
    if(size.y == -1) return 0;
    if(parent != this) return 0;
    if(!end)
        imgList.remove(this);

    if(data != nullptr) delete [] data;
    data = nullptr;
    size.y = -1;

    return 1;
}

void texture::setColor(float r, float g, float b, float a)
{
    color.x = r;
    color.y = g;
    color.z = b;
    if(a != -1) color.w = a;
}

void texture::setAlpha(float a)
{
    if(a < 0) a = 0;
    else if(a > 1) a = 1;
    color.w = a;
}

void texture::setParts(int t, int s)
{
    if(t < 1
    || s < 1) return;
    parts = {t, s};
}

void texture::setAngle(float a)
{
    angle = fmod(a, 360.0)*M_PI/180.0;
    float msin, mcos;
    for(int i = 0; i < 4; i++)
    {
        msin = sinf(angle+M_PI_2*i);
        mcos = cosf(angle+M_PI_2*i);
        rotMatr[i][0] = ((msin-mcos)/2);
        rotMatr[i][1] = ((msin+mcos)/2);
    }
}

void texture::rotate(float a)
{
    angle += fmod(a, 360.0)*M_PI/180.0;
    float msin, mcos;
    for(int i = 0; i < 4; i++)
    {
        msin = sinf(angle+M_PI_2*i);
        mcos = cosf(angle+M_PI_2*i);
        rotMatr[i][0] = ((msin-mcos)/2);
        rotMatr[i][1] = ((msin+mcos)/2);
    }
}

float texture::getAngle()
{
    return angle*180.0/M_PI;
}

void texture::getColorMod(float *r, float *g, float *b)
{
    *r = color.x;
    *g = color.y;
    *b = color.z;
}

void texture::getColorMod(float *r, float *g, float *b, float *a)
{
    *r = color.x;
    *g = color.y;
    *b = color.z;
    *a = color.w;
}

bool texture::load(std::string path, int q)
{
    size = loadTexture(path.c_str(), &tex, q);
    if(size.y == -1) return false;
    parent = this;
    _alloc();
    return true;
}

void texture::opAssign(texture *p)
{
    if(p->parent == this) return;//от самоприсваивания
    if(parent == this) unload(); //имeет изображение
    parent = p->parent;

    tex = p->tex;
    size = p->size;
    scale = p->scale;
    angle = p->angle;
    color = p->color;
    data = p->data;
    parts = p->parts;
    memcpy(rotMatr, p->rotMatr, sizeof(float)*8);
}

void texture::draw(float x, float y, int t, int s)
{
    if(size.y == -1) return;
    if(t < 0
    || s < 0
    || t > parts.x
    || s > parts.y) return;

    float offset[4][2] = {0, 1, 1, 1, 1, 0, 0, 0};
    if(parent->drawCalls+1 > drawImageBufSize) parent->flush();
    for(int i = 0; i < 4; i++)
    {
        parent->data[parent->drawCalls].x = x+(size.x*scale.x*rotMatr[i][0]/parts.x);
        parent->data[parent->drawCalls].y = y+(size.y*scale.y*rotMatr[i][1]/parts.y);

        parent->data[parent->drawCalls].t = (t+offset[i][0])/parts.x;
        parent->data[parent->drawCalls].s = 1.0-(s+offset[i][1])/parts.y;

        parent->data[parent->drawCalls].r = color.x;
        parent->data[parent->drawCalls].g = color.y;
        parent->data[parent->drawCalls].b = color.z;
        parent->data[parent->drawCalls].a = color.w;
        parent->drawCalls++;
    }
}

void texture::flush()
{
    if(size.y == -1) return;
    if(!drawCalls) return;
    /*glBindBuffer(GL_ARRAY_BUFFER, render::VBO[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, drawCalls*sizeof(_imgVert), parent->data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glFinish();

    if(!render::currentShader)
        render::regularShader->bind();

    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(render::VAO[1]);
        glDrawArrays(GL_QUADS, 0, drawCalls);
    glBindVertexArray(0);*/
    drawCalls = 0;
}

void texture::createTarget(int w, int h)
{
    if(w < 1 || h < 1) return;
    size = {w, h};
    if(parent != this)
    {
        _alloc();
        parent = this;
    }
    if(!glIsTexture(tex))
        glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void texture::copyTarget(int x, int y)
{
    if(parent != this) return;
    glBindTexture(GL_TEXTURE_2D, tex);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 0, 0, size.x, size.y);
}

void parametri(int target, int value)
{
    bind();
}
