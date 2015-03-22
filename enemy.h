#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "common.h"
#include "level.h"

class enemy
{
    float hitTime;
    vec2f pos;
    vec2f dest;
    float angle;
    int tangle;
    int health;
    float speed;
    bool alive;
    level* parent;
    float timer;
    void findWay();
public:
    vec2i getPos();
    vec2f getPosf();

    bool isAlive();
    void kill();
    void spawn(int, int, level*, int);
    void update();
    void draw();
};

#endif // ENEMY_H_INCLUDED
