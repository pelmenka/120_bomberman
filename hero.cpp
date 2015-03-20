#include <GLFW/glfw3.h>
#include <math.h>
#include "hero.h"
#include "level.h"
#include "main.h"

#include <stdio.h>

void hero::init()
{
    reset();
    mines.resize(3);
}

void hero::reset()
{
    angle = 0;
    tangle = 0;
    pos = {0, 0};
}

void hero::move(int d)
{
    tangle = d%4;
    switch(-tangle)
    {
    case 0:
        pos.y += 0.1; break;
    case 1:
        pos.x += 0.1; break;
    case 2:
        pos.y -= 0.1; break;
    case 3:
        pos.x -= 0.1; break;
    }
}

void hero::update(int a)
{
    static float time = 0;
    if(time+0.02 > glfwGetTime()) return;
    time = glfwGetTime();

    a += 45;
    a /= -90;

    bool moving = 0;
    int x = round(pos.x), y = round(pos.y);

    if(getKey(GLFW_KEY_W) || getKey(GLFW_KEY_UP))
        move(a);
    if(getKey(GLFW_KEY_S) || getKey(GLFW_KEY_DOWN))
        move(a-2);
    if(getKey(GLFW_KEY_A) || getKey(GLFW_KEY_LEFT))
        move(a-3);
    if(getKey(GLFW_KEY_D) || getKey(GLFW_KEY_RIGHT))
        move(a-1);

    if(getKey(GLFW_KEY_SPACE))
        if(!field.getBlock(x, y))
            for(int i = 0; i < mines.size(); i++)
                if(mines[i].spawn(x, y)) break;

    for(int i = 0; i < mines.size(); i++)
        mines[i].update();



    int chkoff[4][2] = {{-1, 1}, {1, -1}, {-1, -1}, {1, 1}};
    int px, py;
    float mx, my;
    vec2f size = {0.3, 0.3};
    for(int i = 0; i < 4; i++)
    {
        px = round(pos.x+size.x*chkoff[i][0]);
        py = round(pos.y+size.x*chkoff[i][1]);

        if(field.getBlock(px, round(pos.y)) > 0)
        {
            if(chkoff[i][0] > 0)
                acc.x = px-0.5-pos.x-size.x;
            else
                acc.x = px+0.5-pos.x+size.x;
        }

        if(field.getBlock(round(pos.x),py) > 0)
        {
            if(chkoff[i][1] > 0)
                acc.y = py-0.5-pos.y-size.y;
            else
                acc.y = py+0.5-pos.y+size.y;
        }
    }

         if(acc.x > 0.16) acc.x = 0.16;
    else if(acc.x < -0.16) acc.x = -0.16;
         if(acc.y > 0.16) acc.y = 0.16;
    else if(acc.y < -0.16) acc.y = -0.16;

    pos.x += acc.x;
    pos.y += acc.y;


         if(acc.x > 0) acc.x -= 0.02;
    else if(acc.x < 0) acc.x += 0.02;

    if(fabs(acc.x) < 0.02) acc.x = 0;

         if(acc.y > 0) acc.y -= 0.02;
    else if(acc.y < 0) acc.y += 0.02;

    if(fabs(acc.y) < 0.02) acc.y = 0;

         if(angle < tangle*90) angle+=30;
    else if(angle > tangle*90) angle-=30;
}

bool bomb::spawn(int x, int y)
{
    puts("BOMB_S");
    if(glfwGetTime() < timer) return 0;
    if(field.getBlock(x, y) != 0) return 0;
    timer = glfwGetTime()+2;
    pos = {x, y};
    power = 3;
    field.putBomb(x, y, this);
    return 1;
}

bomb::bomb()
{
    puts("BOMB_D");
    timer = -1;
    fire = new emitter[4];
    for(int i = 0; i < 4; i++)
        fire[i].nospawn = 1;
    fire[0].speed = {2, 0, 0};
    fire[1].speed = {0, 2, 0};
    fire[2].speed = {-2, 0, 0};
    fire[3].speed = {0, -2, 0};
    expTime = 0;
}

bomb::~bomb()
{
    delete [] fire;
}

void bomb::update()
{
    for(int i = 0; i < 4; i++) fire[i].update();
    //fire[0].update();
    if(timer < 0) return;
    if(glfwGetTime() < timer) return;
    if(glfwGetTime() < expTime+0.5) return;
    puts("BOMB_U");
    explode();
}

void bomb::explode()
{
    puts("BOMB_E");
    timer = -1;
    expTime = glfwGetTime();
    for(int i = 0; i < 4; i++)
    {
        fire[i].pos = {pos.x, pos.y, 0.25};
        fire[i].spawn();
    }
    field.explosion(pos.x, pos.y, power);
}


