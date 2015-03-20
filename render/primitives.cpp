#include <GL/glew.h>
#include "buffer.h"
#include "shader.h"
#include "render.h"

#include <stdio.h>

typedef struct
{
    vec3f vert;
    vec4f color;
}_linVert;

typedef struct
{
    vec2f vert;
    vec2f tex;
    vec4f color;
}_texVert;

const int lineBufSize = 0xffff;
const int pointBufSize= lineBufSize*2;
const int quadBufSize = 0xffff;
const int textBufSize = quadBufSize;

_linVert *lineData, *pointData;
_texVert *textData, *quadData;

rgba render::primitives::lineColor = {1, 1, 1, 1},
     render::primitives::quadColor = {1, 1, 1, 1},
     render::primitives::pointColor = {1, 1, 1, 1};

int lineCount, quadCount, textCount, pointCount;

vertexArray VAO[2];
vertexBuffer VBO[2];

void render::primitives::reset()
{
    lineCount = 0;
    quadCount = 0;
    textCount = 0;
    pointCount = 0;
}

void render::primitives::flush()
{
    flushQuads();
    flushText();
    flushLines();
    flushPoints();
}

void render::primitives::init()
{

    for(int i = 0; i < 2; i++)
    {
        VAO[i].gen();
        VBO[i].gen();
    }


    ///безтекстурные объекты
    {
        VAO[0].bind();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        VBO[0].bind();
        glVertexPointer(3, GL_FLOAT, 7*sizeof(float), 0);
        glColorPointer(4, GL_FLOAT, 7*sizeof(float), (GLuint*)(3*sizeof(float)));
    }

    ///текстурные объекты
    {
        VAO[1].bind();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        VBO[1].bind();
        glVertexPointer(   2, GL_FLOAT, 8*sizeof(float), 0);
        glTexCoordPointer( 2, GL_FLOAT, 8*sizeof(float), (GLuint*)(2*sizeof(float)));
        glColorPointer(    4, GL_FLOAT, 8*sizeof(float), (GLuint*)(4*sizeof(float)));
    }
    VAO[0].unbind();

    lineData = new _linVert[lineBufSize*2];
    quadData = new _texVert[quadBufSize*4];
    textData = new _texVert[textBufSize*4];
    pointData = new _linVert[pointBufSize];

    VBO[0].bind();
    glBufferData(GL_ARRAY_BUFFER, lineBufSize*2*sizeof(_linVert), 0, GL_STREAM_DRAW);
    VBO[1].bind();
    glBufferData(GL_ARRAY_BUFFER, quadBufSize*4*sizeof(_texVert), 0, GL_STREAM_DRAW);

}

void render::close()
{
    delete [] lineData;
    delete [] quadData;
    delete [] textData;
    delete [] pointData;
}

///================ PRIMITIVES

void render::primitives::line(float x1, float y1, float x2, float y2)
{
    if((lineCount+1) >= lineBufSize) flushLines();
    lineData[lineCount*2  ].vert  = {x1, y1};
    lineData[lineCount*2  ].color = lineColor;
    lineData[lineCount*2+1].vert  = {x2, y2};
    lineData[lineCount*2+1].color = lineColor;
    lineCount++;
}

void render::primitives::point(float x, float y, float z, rgba color)
{
    pointColor = color;
    if((pointCount+1) >= pointBufSize) flushPoints();
    pointData[pointCount].vert = {x, y, z};
    pointData[pointCount].color = pointColor;
    pointCount++;
}

void render::primitives::point(vec3f v, rgba color)
{
    pointColor = color;
    if((pointCount+1) >= pointBufSize) flushPoints();
    pointData[pointCount].vert = v;
    pointData[pointCount].color = pointColor;
    pointCount++;
}

void render::primitives::quad(float x1, float y1, float x2, float y2)
{
    if((quadCount+1) >= quadBufSize) flushQuads();
    const float offset[4][2] = {0, 1, 1, 1, 1, 0, 0, 0};

    for(int i = 0; i < 4; i++ )
    {
        quadData[quadCount*4+i].vert = {x1+x2*offset[i][0], y1+y2*offset[i][1]};
        quadData[quadCount*4+i].tex  = {offset[i][0], offset[3-i][0]};
        quadData[quadCount*4+i].color = quadColor;
    }

    quadCount++;
}

void render::primitives::symbol(vec4<float> v, vec2<char> t, vec4<float> c)
{
    if((textCount+1) >= textBufSize) flushText();
    const float offset[4][2] = {0, 1, 1, 1, 1, 0, 0, 0};
    for(int i = 0; i < 4; i++)
    {
        textData[textCount*4+i].tex = {(t.x+offset[i][0])/32.0, 1.0-(t.y+offset[i][1])/10.0};
        textData[textCount*4+i].vert = {v.x+v.z*offset[i][0], v.y+v.w*offset[i][1]};
        textData[textCount*4+i].color = c;
    }
    textCount++;
}

///================ FLUSH

void render::primitives::flushPoints()
{
    if(!pointCount) return;
    VBO[0].bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, pointCount*sizeof(_linVert), pointData);
    VAO[0].bind();
    glDrawArrays(GL_POINTS, 0, pointCount);
    VAO[0].unbind();
    pointCount = 0;
}

void render::primitives::flushLines()
{
    if(!lineCount) return;
    VBO[0].bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, lineCount*2*sizeof(_linVert), lineData);
    VAO[0].bind();
    glDrawArrays(GL_LINES, 0, lineCount*2);
    VAO[0].unbind();
    lineCount = 0;
}

void render::primitives::flushQuads()
{
    if(!quadCount) return;
    VBO[1].bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, quadCount*4*sizeof(_texVert), quadData);
    VAO[1].bind();
    glDrawArrays(GL_QUADS, 0, quadCount*4);
    VAO[1].unbind();
    quadCount = 0;
}

void render::primitives::flushText()
{
    if(!textCount) return;
    //glBindTexture(GL_TEXTURE_2D, gui::font);

    VBO[1].bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, textCount*4*sizeof(_texVert), textData);
    VAO[1].bind();
    glDrawArrays(GL_QUADS, 0, textCount*4);
    VAO[1].unbind();
    textCount = 0;
}
