#ifndef PRACTICLES_H_INCLUDED
#define PRACTICLES_H_INCLUDED

#include "../common.h"

struct partvert
{
    float r, g, b, a;
    float x, y, z;
};

class emitter;

class particle
{
    friend class emitter;
    bool collide;
    emitter *parent;
    float timer, lifeTime, oldTime;
    vec4f startColor, endColor, color;
    vec3f position, speed, pos;
    particle();
    void spawn();
    bool update();
};

class emitter
{
    friend class particle;
protected:
    particle *data;
    partvert *rdata;
    vec3f getPosition();
    int count;
public:
    bool nospawn = 0;
    float lifeTime;
    vec4f startColor, endColor;
    vec3f pos, speed, size;
    emitter(int = 2048);
    ~emitter();

    void init();
    void update();
    void draw();

    void spawn();
};

#endif // PRACTICLES_H_INCLUDED
