#ifndef PRACTICLES_H_INCLUDED
#define PRACTICLES_H_INCLUDED

#include "../common.h"
#include <vector>

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

    void spawn();
    bool update();
public:
    particle();
};

class emitter
{
    friend class particle;
protected:
    std::vector<particle> data;
    vec3f getPosition();
public:
    bool nospawn = 0;
    float lifeTime;
    vec4f startColor, endColor;
    vec3f pos, speed, size;
    emitter(int = 2048);

    void init();
    void update();
    void draw();
    void resize(int);

    void spawn();
};

#endif // PRACTICLES_H_INCLUDED
