#include <GL/glew.h>
#include "level.h"
#include "main.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include "render/render.h"

level::level()
{
    effect[0].resize(512);
    effect[0].startColor = {1, 1, 1, 0.1};
    effect[0].endColor = {1, 1, 1, 0};
    effect[0].speed = {0, 0, 0.4};
    effect[0].pos = {0, 10, 0};
    effect[0].lifeTime = 0.3;

    effect[1].resize(128);
    effect[1].speed = {0, 0, 0};
    effect[1].pos = {0+0.3, 9+0.3, 0};
    effect[1].size = {0.4, 0.4, 0.2};
    effect[1].lifeTime = 1;
}

void level::reset()
{
    count = -1;
    generate();
}

void level::generate()
{
    count++;
    memset(data, 0, 15*11*sizeof(block));
    std::vector<vec2i> pos;
    std::vector<vec2i> fpos;
    vec2i tpos;
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 11; j++)
        {
            if(i%2 && j%2) data[i][j].val = 1;
            else if(i > 2 || j < 9)
            {
                if(!(rand()%4))
                {
                    data[i][j].val = 2;
                    tpos = {i, j};
                    fpos.push_back(tpos);
                }
                else
                {
                    tpos = {i, j};
                    pos.push_back(tpos);
                }
            }
        }
    int enemies = rand()%6+4, index;
    enemy tempEnemy;
    std::random_shuffle(pos.begin(), pos.end());
    std::random_shuffle(fpos.begin(), fpos.end());
    for(int i = 0; i < enemies; i++)
    {
        tempEnemy.spawn(pos[i].x, pos[i].y, this, count);
        army.push_back(tempEnemy);
    }
    exit = fpos[0];
    bonus = fpos[1];

    effect[0].pos = {fpos[0].x, fpos[0].y, 0};
    effect[1].pos = {bonus.x+0.3, bonus.y+0.3, 0};

    printf("EXIT %d %d\n", exit);
    printf("BONUS %d %d\n", bonus);
    printf("LEVEL %d\n", count);

    switch(count%3)
    {
    case 0:
        effect[1].startColor = {1, 0.5, 0.2, 1};
        effect[1].endColor = {1, 0, 0, 0}; break;
    case 1:
        effect[1].startColor = {.388, .608, 1, 1};
        effect[1].endColor = {.608, .388, 1, 0}; break;

    case 2:
        effect[1].startColor = {.416, .745, .188, 1};
        effect[1].endColor = {.196, .235, .224, 0}; break;
    }
    canExit = 0;
    bonusTaken = 0;
    render::game::resetTerrain();
    mathShadows();

}

void level::update()
{
    effect[0].update();

    canExit = 1;
    vec2f enPos, aPos = actor.getPos();
    for(int i = 0; i < army.size(); i++)
    {
        if(!army[i].isAlive()) continue;
        canExit = 0;
        army[i].update();
        if(actor.isAlive())
        {
            enPos = army[i].getPosf();
            if((enPos.x-aPos.x)*(enPos.x-aPos.x)+(enPos.y-aPos.y)*(enPos.y-aPos.y) < 0.44) // 0.44 == sqrtf(0.2)
                actor.kill();
        }
    }
    if(canExit)
    {
        effect[0].speed = {0, 0, 0.4};
        effect[0].startColor.w = 0.3;
        vec2f aPos;
        aPos.x = actor.getPos().x;
        aPos.y = actor.getPos().y;

        if(round(aPos.x) == exit.x
        && round(aPos.y) == exit.y)
        {
            generate();
            actor.spawn();
        }
    }
    else
    effect[0].speed = {0, 0, 0};
    if(!bonusTaken)
    {
        effect[1].update();
        vec2f aPos;
        aPos.x = actor.getPos().x;
        aPos.y = actor.getPos().y;

        if(round(aPos.x) == bonus.x
        && round(aPos.y) == bonus.y)
        {
            bonusTaken = 1;
            switch(count)
            {
            case 0:
                actor.addPower(); break;
            case 1:
                actor.addMine(); break;
            case 2:
                actor.addHealth(); break;
            }

        }
    }

}

void level::drawArmy()
{
    for(int i = 0; i < army.size(); i++)
        army[i].draw();
    if(getBlock(exit.x, exit.y) != 2)
        effect[0].draw();
    if(getBlock(bonus.x, bonus.y) != 2 && !bonusTaken)
        effect[1].draw();
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
    vec2f aPos;
    aPos.x = actor.getPos().x;
    aPos.y = actor.getPos().y;

    checkArmy(x, y);
    if(round(aPos.x) == x
    && round(aPos.y) == y) actor.kill();

    render::game::putBlock(x, y, 3);
    data[x][y].ref = 0;
    data[x][y].val = 1;
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
            b[i]++;
            checkArmy(r.x, r.y);
            if(round(aPos.x) == r.x
            && round(aPos.y) == r.y) actor.kill();

            render::game::putBlock(r.x, r.y, 2);
            if(data[r.x][r.y].ref)
            {
                data[r.x][r.y].ref->explode();
                break;
            }


            if(data[r.x][r.y].val == 2)
            {
                data[r.x][r.y].val = 0;
                break;
            }
        }
    }
    data[x][y].val = 0;
    vec4i sides;
    memcpy(&sides, b, sizeof(vec4i));
    return sides;
}

void level::checkArmy(int x, int y)
{
    vec2f enPos;
    for(int i = 0; i < army.size(); i++)
    {
        if(!army[i].isAlive()) continue;
        enPos = army[i].getPosf();
        if(round(enPos.x) == x
        && round(enPos.y) == y) army[i].kill();
    }
}

void level::putBomb(int x, int y, bomb *r)
{
    if(x < 0
    || x > 14
    || y < 0
    || y > 10) return;
    data[x][y].ref = r;
}
