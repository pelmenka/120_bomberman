#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "common.h"
#include "level.h"

class enemy
{
    vec2f pos;
    vec2f dest;
    float angle;
    int tangle;
    bool alive;
    level* parent;
    float timer;
    void findWay();
public:
    vec2i getPos();
    void kill();
    void spawn(int, int, level*);
    void update();
    void draw();
};

#endif // ENEMY_H_INCLUDED
