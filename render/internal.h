#ifndef RENDER_INTERNAL_H_INCLUDED
#define RENDER_INTERNAL_H_INCLUDED

#include "shader.h"
#include "model.h"

#define TEX_COUNT 9

namespace internal
{
    extern shader defaultShader, terrainShader, wallShader, particleShader;
    extern model crateModel, heroModel, bombModel, enemyModel;
    extern unsigned int textures[TEX_COUNT];
    void loadResources();
}

#endif // RENDER_INTERNAL_H_INCLUDED
