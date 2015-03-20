#ifndef ASIMAGE_H_INCLUDED
#define ASIMAGE_H_INCLUDED

#include "../common.h"


#if defined(__gl_h_) || defined(__GL_H__) || defined(__X_GL_H)

#include <string>

typedef struct
{
    float x, y; //vert
    float t, s; //tex
    float r, g, b, a; //color
}_imgVert;

class texture
{
    GLuint tex;
    int refCount;
    texture *parent;
    void _alloc();
    int slot;
public:


    vec4<float> color;
    vec2<float> scale;
    vec2<short> parts;
    vec2<long> size;
    float angle, rotMatr[4][2];

    _imgVert *data;
    int drawCalls;


    texture();
    texture(std::string, int = 3);
    ~texture();

    void bind(int = 0);
    void unbind();

    bool load(std::string, int  = 3);
    bool unload(bool = false);

    void draw(float, float, int, int);
    void flush();

    void setScale(float x, float y){scale = {x, y};}
    void setAlpha(float);
    void setColor(float, float, float, float);
    void setAngle(float);
    void setParts(int, int);
    void rotate(float);

    void getColorMod(float*, float*, float*);
    void getColorMod(float*, float*, float*, float*);
    void getSize(int *x, int *y){*x = size.x; *y = size.y;}
    void getParts(int*x, int*y){*x = parts.x; *y = parts.y;}
    void getScale(float *x, float *y){*x = scale.x; *y = scale.y;}
    void getAlpha(float *a){*a = color.w;}
    float getAngle();

    void parametri(int, int);

    void createTarget(int, int);
    void copyTarget(int, int);

    void opAssign(texture*);
    operator=(texture* s){opAssign(s);}
};

#endif // defined

void flushImages();

#endif // ASIMAGE_H_INCLUDED
