#include <GL/glew.h>
#include "internal.h"
#include "shader.h"
#include "model.h"
#include "../texops.h"

using namespace internal;

GLuint internal::textures[TEX_COUNT];
shader internal::defaultShader, internal::terrainShader, internal::wallShader, internal::particleShader;
model internal::crateModel, internal::heroModel, internal::bombModel, internal::enemyModel;

void loadTextures();
void loadShaders();
void loadModels();

void internal::loadResources()
{
    loadTextures();
    loadShaders();
    loadModels();
}

void loadTextures()
{
    loadTexture("data/textures/mask.png", &textures[0], 3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    loadTexture("data/textures/grass.jpg", &textures[1], 7);
    loadTexture("data/textures/rock.jpg", &textures[2], 7);
    loadTexture("data/textures/lava.jpg", &textures[3], 7);
    loadTexture("data/textures/dirt.jpg", &textures[4], 7);
    loadTexture("data/textures/shadowmap.png", &textures[5], 3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    for(int i = 0; i < 6; i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    glActiveTexture(GL_TEXTURE0);
    loadTexture("data/textures/particle.png", &textures[6], 0);
    loadTexture("data/textures/stonewall.jpg", &textures[7], 7);
    loadTexture("data/textures/concrete.jpg", &textures[8], 7);
}

void loadShaders()
{
    terrainShader.load("data/shaders/terrain.vert", "data/shaders/terrain.frag");
    wallShader.load("data/shaders/wall.vert", "data/shaders/wall.frag");
    particleShader.load("data/shaders/particles.vert", "data/shaders/particles.frag");
    defaultShader.load("data/shaders/default.vert", "data/shaders/default.frag");

    const char *names[] = {"mask", "grass", "rock", "lava", "dirt", "shadowmap"};

    int loc;
    terrainShader.bind();
    for(int i = 0; i < 6; i++)
    {
        loc = glGetUniformLocation(terrainShader.program, names[i]);
        glUniform1i(loc, i);
    }
    terrainShader.unbind();

}

void loadModels()
{
    enemyModel.load("data/models/enemy/enemy.obj");
    bombModel.load("data/models/Bomb/Bomb.obj");
    heroModel.load("data/models/Lilith/Lilith.obj");
    crateModel.load("data/models/Crate/Crate.obj");

}
