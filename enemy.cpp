#include <GLFW/glfw3.h>
#include "enemy.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int round(float s)
{
    if(s-(int)s > 0.99) return (int)s+1;
    return (int)s;
}

bool closeTo(float a, float b, float e = 0.01)
{
    return (fabs(a-b) < e);
}

void enemy::spawn(int x, int y, level *p)
{
    pos = {x, y};
    dest = {x, y};
    parent = p;
    alive = 1;
    timer = glfwGetTime();
    angle = 0;
    tangle = 0;
}

void enemy::update()
{
    if(!alive)
        return;


    float delta = glfwGetTime() - timer;
    timer = glfwGetTime();
    if(closeTo(pos.x, round(dest.x))
    && closeTo(pos.y, round(dest.y)))
        findWay();

         if(pos.x < dest.x) pos.x += 0.6*delta;
    else if(pos.x > dest.x) pos.x -= 0.6*delta;

         if(pos.y < dest.y) pos.y += 0.6*delta;
    else if(pos.y > dest.y) pos.y -= 0.6*delta;


    float add;
         if(angle < tangle*90)
         {
             add = 360*delta;
             if(tangle*90-angle < add)
                add = tangle*90-angle;
             angle += add;
         }
    else if(angle > tangle*90)
        {
             add = 360*delta;
             if(angle-tangle*90 < add)
                add = angle-tangle*90;
             angle -= add;
         }
}

void enemy::kill()
{
    alive = 0;
}

vec2i enemy::getPos()
{
    return {round(pos.x), round(pos.y)};
}

void enemy::findWay()
{
    int way[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::vector<int> available;
    for(int i = 0; i < 4; i++)
    {
        if(parent->getBlock(round(pos.x)+way[i][0], round(pos.y)+way[i][1]) == 0)
            available.push_back(i)
    }


    if(available.empty()) return;
    tangle = available[rand()%available.size()];
    dest.x = pos.x+way[tangle][0];
    dest.y = pos.y+way[tangle][1];

}
