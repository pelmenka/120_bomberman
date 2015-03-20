#include <GL/glew.h>
#include "level.h"
#include <memory.h>
#include <stdlib.h>
#include "render/render.h"

level::level()
{
    generate();
}

void level::generate()
{
    memset(data, 0, 15*11*sizeof(block));
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 11; j++)
        {
            if(i%2 && j%2) data[i][j].val = 1;
            else if(i > 2 || j > 2)
            if(!(rand()%4)) data[i][j].val = 2;
        }
}

char level::getBlock(int x, int y)
{
    if(x < 0
    || y < 0
    || x > 14
    || y > 10) return 3;

    if(data[x][y].ref) return -1;
    return data[x][y].val;
}

vec4i level::explosion(int x, int y, int p)
{
    int off[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    int b[4] = {0, 0, 0, 0};


    render::game::putBlock(x, y, 3);
    data[x][y].ref = 0;
    vec2i r;
    for(int i = 0; i < 4; i++)
    {
        r = {x, y};
        for(int j = 1; j < p; j++)
        {
            r.x += off[i][0];
            r.y += off[i][1];
            if(r.x < 0
            || r.x > 14
            || r.y < 0
            || r.y > 10) break;

            if(data[r.x][r.y].val == 1) break;
            render::game::putBlock(r.x, r.y, 2);
            if(data[r.x][r.y].ref)
                data[r.x][r.y].ref->explode();

            if(data[r.x][r.y].val == 2)
            {
                data[r.x][r.y].val = 0;
                break;
            }
            b[i]++;
        }
    }
    vec4i sides;
    memcpy(&sides, b, sizeof(vec4i));
    return sides;
}

void level::putBomb(int x, int y, bomb *r)
{
    if(x < 0
    || x > 14
    || y < 0
    || y > 10) return;
    data[x][y].ref = r;
}
