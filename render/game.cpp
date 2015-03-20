#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory.h>
#include "internal.h"
#include "render.h"
#include "buffer.h"
#include "model.h"
#include "../level.h"

char expMap[45][33];

vertexArray levelVAO;
vertexBuffer levelVBO;

const int MAX_LEVEL_BUFFER_SIZE = (15*11-7*5)*4+7*5*5*4;

void resetTerrain(level*);

void level::init()
{
    levelVAO.gen();
    levelVBO.gen();

    levelVAO.bind();
        levelVBO.bind();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), (GLuint*)(3*sizeof(float)));
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), 0);
    levelVAO.unbind();
    glBufferData(GL_ARRAY_BUFFER, MAX_LEVEL_BUFFER_SIZE*sizeof(vertex), 0, GL_STATIC_DRAW);

    const float off[4][2] = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
    vertex *data = new vertex[MAX_LEVEL_BUFFER_SIZE];

    int count = 0;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 11; j++)
        {
            if(i%2 && j%2) continue;
            for(int k = 0; k < 4; k++)
            {
                data[count].tex = {i+off[3-k][0], j+off[k][1]};
                data[count].vert = {i+off[3-k][0], j+off[k][1], 0};
                count++;
            }
        }

    for(int i = 0; i < 7; i++)
        for(int j = 0; j < 5; j++)
            for(int k = 0; k < 4; k++)
                for(int n = 0; n < 4; n++)
                {
                    data[count].tex = {off[n][0], off[n][1]};
                    switch(k)
                    {
                    case 0:
                        data[count].vert = {i*2+1, (j*2)+off[n][0]+1, off[n][1]}; break;
                    case 1:
                        data[count].vert = {i*2+2, (j*2)+off[3-n][0]+1, off[n][1]}; break;
                    case 2:
                        data[count].vert = {i*2+1+off[3-n][0], (j*2)+1, off[n][1]}; break;
                    case 3:
                        data[count].vert = {i*2+1+off[n][0], (j*2)+2, off[n][1]}; break;
                    }
                    count++;
                }

    for(int i = 0; i < 7; i++)
        for(int j = 0; j < 5; j++)
        {
            for(int k = 0; k < 4; k++)
            {
            data[count].tex = {off[k][0], off[k][1]};
            data[count].vert = {i*2+1+off[k][1], (j*2)+off[k][0]+1, 1};
            count++;}
        }
    glBufferSubData(GL_ARRAY_BUFFER, 0, count*sizeof(vertex), data);
    delete [] data;
    resetTerrain(this);
}

void level::draw()
{
    mathShadows();
    internal::terrainShader.bind();
    glBindTexture(GL_TEXTURE_2D, internal::textures[0]);

    levelVAO.bind();
        glDrawArrays(GL_QUADS, 0, 520); //terrain

    internal::wallShader.bind();
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, internal::textures[7]);
        glDrawArrays(GL_QUADS, 520, 4*7*5*4); //walls

    glBindTexture(GL_TEXTURE_2D, internal::textures[8]);
        glDrawArrays(GL_QUADS, 520+4*7*5*4, 4*7*5); //walls top

    levelVAO.unbind();
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 11; j++)
            if(getBlock(i, j) == 2)
            {
                glPushMatrix();
                glTranslatef(i, j, 0);
                internal::crateModel.draw();
                glPopMatrix();
            }
}

void resetTerrain(level *field)
{
    memset(expMap, 0, 45*33);
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 11; j++)
            switch(field->getBlock(i, j))
            {
            case 0: render::game::putBlock(i, j, 0); break;
            case 1: render::game::putBlock(i, j, 2); break;
            case 2: render::game::putBlock(i, j, 1); break;
            }
}

void level::mathShadows()
{
    struct pix
    {
        uchar r;
        uchar g, b, a;
    };
    pix *tex = new pix[45*33];

    int offset[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {-1, 1}, {1, -1}, {-1, -1}, {1, 1}};

    memset(tex, 0, 45*33*sizeof(pix));
    uchar shad;
    for(int i = 0; i < 33; i++)
        for(int j = 0; j < 45; j++)
        {
            shad = 0;
            if(getBlock(j/3, i/3) == 1
            || getBlock(j/3, i/3) == 2)
                shad = 128;
            else
                for(int k = 0; k < 4; k++)
                    switch(getBlock((j+offset[k][0])/3, (i+offset[k][1])/3))
                    {
                    case 1:
                    case 2:
                        shad = 96;
                        break;
                    }

            if(!shad)
            {
                for(int k = 4; k < 8; k++)
                    switch(getBlock((j+offset[k][0])/3, (i+offset[k][1])/3))
                    {
                    case 1:
                    case 2:
                        shad = 64;
                        break;
                    }
            }

            tex[i*45+j].r = shad;

        }
    glBindTexture(GL_TEXTURE_2D, internal::textures[5]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 45, 33, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);
    delete [] tex;
}

void hero::draw()
{
    internal::defaultShader.bind();
    glPushMatrix();
    glTranslatef(pos.x+0.5, pos.y+0.5, 0);
    glScalef(0.3, 0.3, 0.3);

    glRotatef(90, 1, 0, 0);
    glRotatef(angle+180, 0, 1, 0);
    internal::heroModel.draw();
    glPopMatrix();
    for(int i = 0; i < mines.size(); i++)
        mines[i].draw();
}

void bomb::draw()
{
    if(timer > 0)
    {
        float scale = 0.05+0.01*(2-timer+glfwGetTime());
        internal::defaultShader.bind();
        glPushMatrix();
        glTranslatef(pos.x+0.5, pos.y+0.5, 0);
        glScalef(scale, scale, scale);
        glRotatef(90, 1, 0, 0);
        internal::bombModel.draw();
        glPopMatrix();
    }

    for(int i = 0; i < 4; i++)
        fire[i].draw();
}

void render::game::putPix(int x, int y, int col)
{
    const uint colors[4] = {0xFF000000, //BLACK
                            0xFFFF0000, //BLUE
                            0xFF0000FF, //RED
                            0xFF00FFFF};//YELLOW

    int val = col;
    if(val <= expMap[x][y]) return;
    expMap[x][y] = val;
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &colors[col]);
}

void render::game::putBlock(int x, int y, int col)
{
    glBindTexture(GL_TEXTURE_2D, internal::textures[0]);
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            putPix(x*3+i, y*3+j, col);
}
