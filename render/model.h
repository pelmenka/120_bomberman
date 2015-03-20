#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <string>
#include "../common.h"

typedef struct
{
    int v[3];
    int t[3];
}triangle;

struct vertex
{
    vec3f vert;
    vec2f tex;
};

typedef struct
{
    std::string name;
    uint tex;
    int count;
    int offset;
}material;

class model
{
    uint VAO, VBO;
    material *materials;
    int matCount;

    bool mathmtl(std::string, std::string);
    int findmtl(std::string);
public:
    model();
    ~model();

    bool load(std::string);
    void draw();
    void drawOld();
};

#endif // MODEL_H_INCLUDED
