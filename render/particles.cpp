#include "particles.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <random>
#include <assert.h>
#include "render.h"

using std::default_random_engine;

default_random_engine randEngine;

emitter::emitter(int s)
{
    resize(s);

    startColor = {1, 0.5, 0.2, 1};
    endColor = {1, 0, 0, 0};
    pos = {0, 0, 0.25};
    speed = {0.2, 0, 0};

    lifeTime = 0.5;
    size = {1, 1, 0.5};
    nospawn = 0;
}

void emitter::resize(int s)
{
    data.resize(s);
    for(int i = 0; i < s; i++)
    {
        data[i].parent = this;
        data[i].spawn();
    }

}

vec3f emitter::getPosition()
{
    vec3f p = pos;
    p.x += (float(randEngine())/randEngine.max())*size.x;
    p.y += (float(randEngine())/randEngine.max())*size.y;
    p.z += (float(randEngine())/randEngine.max())*size.z;
    return p;
}

void emitter::update()
{
    for(int i = 0; i < data.size(); i++)
        if(data[i].update() && !nospawn)
            data[i].spawn();
}

void emitter::spawn()
{
    //if(!nospawn) return;
    for(int i = 0; i < data.size(); i++)
        data[i].spawn();
}

///========================================================================

particle::particle()
{
    timer = 0;
    parent = 0;
    collide = 0;
    position = {0, 0, 0};
    speed = {0, 0, 0};
    color = {0, 0, 0, 0};
    startColor = {0, 0,0, 0};
    endColor = {0, 0, 0, 0};
    lifeTime = 0;
    oldTime = 0;
}

void particle::spawn()
{
    startColor = parent->startColor;
    endColor = parent->endColor;
    position = parent->getPosition();
    speed = parent->speed;
    lifeTime = parent->lifeTime;//*(float(rand()%100+900.0)/1000.0);
    //printf("LIFE %f\n", lifeTime);
    float mod = float(rand()%1000)/1000.0;
    timer = mod*lifeTime;
    //position.x += speed.x
    //printf("%f\n", timer);
}

bool particle::update()
{
    timer += glfwGetTime()-oldTime;
    oldTime = glfwGetTime();
    if(lifeTime == 0.0) return 0;
    float mul = timer/lifeTime;

    //printf("%f %f %f\n", glfwGetTime(), lifeTime, mul);
    if(mul > 2) return 1;


    if(mul < 0) mul = 0;
    pos.x = position.x + speed.x*mul;
    pos.y = position.y + speed.y*mul;
    pos.z = position.z + speed.z*mul;
    color.x = startColor.x + (endColor.x-startColor.x)*mul;
    color.y = startColor.y + (endColor.y-startColor.y)*mul;
    color.z = startColor.z + (endColor.z-startColor.z)*mul;
    color.w = startColor.w + (endColor.w-startColor.w)*mul;
    if(mul > 1) return 1;
    return 0;
}
