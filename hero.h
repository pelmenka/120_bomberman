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
    emitter fire[4];
public:
    int power;
    bomb();
    bool spawn(int, int, int);
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
    bool alive;
    int power;
    int health;
    float hitTime;

    void move(int, float);
public:
    const vec2f getPos(){return pos;}

    void spawn();
    void init();
    void reset();

    bool isAlive();
    void kill();
    void update(int);
    void draw();

    void addMine();
    void addPower();
    void addHealth();
};

#endif // HERO_H_INCLUDED
