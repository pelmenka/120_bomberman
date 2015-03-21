#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <string.h>
#include "model.h"
#include "../texops.h"
#include <fstream>

using namespace std;

void getString(char*, int, FILE*);

model::model()
{

}

model::~model()
{

}

int getFirstString(const char *in, char *out)
{
    for(int i = 0; i < strlen(in); i++)
        if(in[i] == '\0'
        || (in[i] == ' ' && i))
        {
            memcpy(out, in, i);
            out[i] = 0;
            return i;
        }
}

bool model::load(string path)
{
    vector<vec3f> vert;
    vector<vec2f> texCoord;
    vector<vec2i> indecies;
    vector<vertex> data;

    ifstream obj(path);
    char temp[1024];
    char temp2[256];

    if(obj.bad()) return 0;

    vec3f tempVert;
    vec2f tempTexCoord;
    vec2i tempIndex[3];
    int currentMaterial;

    while(!obj.eof())
    {
        obj.getline(temp, sizeof(temp));
        getFirstString(temp, temp2);
        if(!strcmp("v", temp2))
        {
            sscanf(temp, "v %f %f %f", &tempVert.x, &tempVert.y, &tempVert.z);
            vert.push_back(tempVert);
        }
        else if(!strcmp("vt", temp2))
        {
            sscanf(temp, "vt %f %f", &tempTexCoord.x, &tempTexCoord.y);
            texCoord.push_back(tempTexCoord);
        }
        else if(!strcmp("f", temp2))
        {
            sscanf(temp, "f %d/%d %d/%d %d/%d",
                &tempIndex[0].x, &tempIndex[0].y,
                &tempIndex[1].x, &tempIndex[1].y,
                &tempIndex[2].x, &tempIndex[2].y);
            for(auto i: tempIndex)
                indecies.push_back(i);
            offsets[currentMaterial].y += 3; //x - offset, y - count
        }
        else if(!strcmp("mtllib", temp2))
        {
            sscanf(temp, "mtllib %s", temp2);
            mathmtl(path, temp2);
        }
        else if(!strcmp("usemtl", temp2))
        {
            sscanf(temp, "usemtl %s", temp2);
            currentMaterial = findmtl(temp2);
            offsets[currentMaterial].x = indecies.size();
        }
    }
    obj.close();

    int index, tindex;
    int count = 0;
    data.resize(indecies.size());
    for(int i = 0; i < indecies.size(); i++)
    {
        index = indecies[i].x-1;
        tindex = indecies[i].y-1;
        data[i].tex = texCoord[tindex];
        data[i].vert = vert[index];
        //count++;
    }

    VAO.gen();
    VBO.gen();

    VAO.bind();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        VBO.bind();
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), 0);
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), (GLuint*)(3*sizeof(float)));
    VAO.unbind();

    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(vertex), data.data(), GL_STATIC_DRAW);
    VBO.unbind();


    return 1;
}

bool model::mathmtl(string path, string name)
{

    char temp[1024];
    char temp2[256];
    int pos = path.rfind('\\');
    if(pos == -1) pos = path.rfind('/');
    pos++;
    path.erase(pos);
    name = path+name;
    ifstream mtl(name);
    material tempMat = {" ", 0};
    pos = -1;

    while(!mtl.eof())
    {
        mtl.getline(temp, sizeof(temp));
        getFirstString(temp, temp2);
        if(!strcmp("newmtl", temp2))
        {
            pos++;
            materials.push_back(tempMat);
            sscanf(temp, "newmtl %s", temp2);
            materials[pos].name = temp2;
        }
        else if(!strcmp("map_Kd", temp2))
        {
            sscanf(temp, "map_Kd %s", temp2);
            loadTexture(path+temp2, &materials[pos].tex);
        }
    }

    offsets.resize(materials.size());
    for(int i = 0; i < offsets.size(); i++)
        offsets[i] = {0, 0};
    mtl.close();
}

int model::findmtl(string name)
{
    for(int i = 0; i < materials.size(); i++)
        if(materials[i].name == name) return i;
    return -1;
}

void model::draw()
{
    glEnable(GL_TEXTURE_2D);
    VAO.bind();
        for(int i = 0; i < offsets.size(); i++)
        {
            glBindTexture(GL_TEXTURE_2D, materials[i].tex);
            glDrawArrays(GL_TRIANGLES, offsets[i].x, offsets[i].y);
        }
    VAO.unbind();
}
