#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "../common.h"

typedef vec4f rgba;

namespace render
{
    void clearScreen();
    void mode2D();
    void mode3D();
    void endFrame();

    namespace primitives
    {
        extern rgba pointColor;
        extern rgba lineColor;
        extern rgba quadColor;

        void point(float, float, float = 0, rgba = pointColor);
        void point(vec3f, rgba = pointColor);
        void line(float, float, float, float);
        void quad(float, float, float, float);
        void symbol(vec4<float>, vec2<char>, vec4<float>);

        void flushPoints();
        void flushLines();
        void flushQuads();
        void flushText();

        void reset();
        void flush();

        void init();
        void close();
    }

    namespace game
    {
        void putPix(int, int, int);
        void putBlock(int, int, int);
        void drawParticles();
    }

    void init();
    void close();
}

#endif // RENDER_H_INCLUDED
