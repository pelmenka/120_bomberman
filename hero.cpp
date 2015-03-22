#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>
#include "hero.h"
#include "level.h"
#include "main.h"

#include <stdio.h>

void hero::init()
{
    reset();
}

void hero::reset()
{
    mines.reserve(60);
    mines.resize(1);
    power = 2;
    alive = 1;
    health = 3;
    spawn();
}

void hero::spawn()
{
    angle = 180;
    tangle = 2;
    pos = {0, 10};
    hitTime = glfwGetTime();
}

void hero::addHealth()
{
    health++;
}

void hero::addMine()
{
    bomb temp;
    mines.push_back(temp);
    for(int i = 0; i < 4; i++)
        mines.back().fire[i].resize(2048);
}

void hero::addPower()
{
    power++;
}

void hero::move(int d, float delta)
{
    tangle = d%4;
    switch(-tangle)
    {
    case 0:
        pos.y += 3*delta; break;
    case 1:
        pos.x += 3*delta; break;
    case 2:
        pos.y -= 3*delta; break;
    case 3:
        pos.x -= 3*delta; break;
    }
}

void hero::update(int a)
{
    static float time = 0;
    float delta = glfwGetTime() - time;
    time = glfwGetTime();

    for(int i = 0; i < mines.size(); i++)
        mines[i].update();
    if(!alive) return;
    a += 45;
    a /= -90;

    bool moving = 0;
    int x = round(pos.x), y = round(pos.y);

    if(getKey(GLFW_KEY_W) || getKey(GLFW_KEY_UP))
        move(a, delta);
    if(getKey(GLFW_KEY_S) || getKey(GLFW_KEY_DOWN))
        move(a-2, delta);
    if(getKey(GLFW_KEY_A) || getKey(GLFW_KEY_LEFT))
        move(a-3, delta);
    if(getKey(GLFW_KEY_D) || getKey(GLFW_KEY_RIGHT))
        move(a-1, delta);

    if(getKey(GLFW_KEY_SPACE))
        if(!field.getBlock(x, y))
            for(int i = 0; i < mines.size(); i++)
                if(mines[i].spawn(x, y, power)) break;





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

void hero::kill()
{
    if(hitTime+1 > glfwGetTime()) return;
    hitTime = glfwGetTime();
    if(!health) alive = 0;
    else health--;
}

bool hero::isAlive()
{
    return alive;
}

bool bomb::spawn(int x, int y, int p)
{
    if(timer > 0) return 0;
    if(field.getBlock(x, y) != 0) return 0;
    if(glfwGetTime() < expTime+0.2) return 0;
    timer = glfwGetTime()+2;
    pos = {x, y};
    power = p;
    field.putBomb(x, y, this);
    return 1;
}

bomb::bomb()
{
    pos = {0, 0};
    timer = -1;
    for(int i = 0; i < 4; i++)
    {
        fire[i].nospawn = 1;
        fire[i].speed = {0, 0, 0};
    }
    expTime = 0;
}

void bomb::update()
{
    for(int i = 0; i < 4; i++) fire[i].update();
    if(timer < 0) return;
    if(glfwGetTime() < timer) return;
    explode();
}

void bomb::explode()
{
    if(timer < 0) return;

    timer = -1;
    expTime = glfwGetTime();
    vec4i sides = field.explosion(pos.x, pos.y, power);
    fire[0].speed.x = sides.y;
    fire[1].speed.y = sides.x;
    fire[2].speed.x = -sides.w;
    fire[3].speed.y = -sides.z;

    for(int i = 0; i < 4; i++)
    {
        if(abs(fire[i].speed.x+fire[i].speed.y) > 0)
        {
            fire[i].pos = {pos.x, pos.y, 0.25};
            fire[i].spawn();
        }
    }
    soundBox.playSound("data/sounds/explosion.wav");
}


