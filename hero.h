#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "common.h"
#include "render/particles.h"

#include <vector>

class hero;

class bomb
{
    friend class hero;
    vec2f pos;
    float timer = 0, expTime;
    emitter *fire = 0;
public:
    int power;
    bomb();
    ~bomb();
    bool spawn(int, int);
    void update();
    void explode();
    void draw();
};

class hero
{
    vec2f pos;
    vec2f acc;
    float angle;
    int tangle;
    std::vector<bomb> mines;
public:
    const vec2f getPos(){return pos;}

    void move(int);

    void init();
    void reset();

    void update(int);
    void draw();
};

#endif // HERO_H_INCLUDED
