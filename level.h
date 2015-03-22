#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

class level;
class enemy;

#include "common.h"
#include "hero.h"
#include "enemy.h"

#define BLOCK_EMPTY 0
#define BLOCK_SOLID 1
#define BLOCK_BOX 2

struct block
{
    bomb *ref;
    char val;
};

class level
{
    block data[15][11];
    int boxCount;
    int shadowCount, shadowOffset;
    uint VBO, VAO;
    void mathShadows();

    std::vector<enemy> army;
    void checkArmy(int, int);
    vec2i exit, bonus;
    emitter effect[2];
    bool canExit, bonusTaken;
    int count;
public:
    level();

    char getBlock(int, int);
    void putBomb(int, int, bomb*);
    vec4i explosion(int, int, int);

    void generate();
    void reset();
    void update();
    void placeBox();
    void init();
    void draw();
    void drawArmy();
};

#endif // LEVEL_H_INCLUDED
