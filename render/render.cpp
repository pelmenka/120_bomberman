#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "render.h"
#include "shader.h"
#include "model.h"
#include "particles.h"
#include "internal.h"

void render::init()
{
    primitives::init();
    internal::loadResources();
}

void emitter::draw()
{
    for(int i = 0; i < count; i++)
    {
        if(data[i].color.w < 0.01) continue;
        render::primitives::point(data[i].pos, data[i].color);
    }
}

void render::game::drawParticles()
{
    glDepthMask(0);
    glEnable(GL_BLEND);
    glPointSize(32);
    glBindTexture(GL_TEXTURE_2D, internal::textures[6]);
    glEnable(GL_POINT_SPRITE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    render::primitives::flushPoints();
    glDisable(GL_BLEND);
    glDepthMask(1);
}
