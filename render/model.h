#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <string>
#include <vector>
#include "../common.h"
#include "buffer.h"

struct vertex
{
    vec3f vert;
    vec2f tex;
};

typedef struct
{
    std::string name;
    uint tex;
}material;

class model
{
    vertexBuffer VBO;
    vertexArray VAO;
    std::vector<material> materials;
    std::vector<vec2i> offsets;

    bool mathmtl(std::string, std::string);
    int findmtl(std::string);
public:
    model();
    ~model();

    bool load(std::string);
    void draw();
};

#endif // MODEL_H_INCLUDED
