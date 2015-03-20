#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <string.h>
#include "model.h"
#include "../texops.h"

using namespace std;

model::model()
{
    materials = 0;
}

model::~model()
{
    if(materials)
        delete [] materials;
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

void decodeIndex(const char*, int*, int*, int*);

bool model::load(string path)
{
    string line;
    string mtlLib;
    int strOff;
    bool mtlFound = 0;
    FILE *objFile = fopen(path.c_str(), "rt");
    puts(path.c_str());
    if(!objFile) return 0;

    vec3f *vertData;
    vec2f *texData;
    triangle *polygones;

    char temp[1024], temp2[32];
    int vert = 0, tex = 0, poly = 0;
    while(!feof(objFile))
    {
        fgets(temp, 1024, objFile);
        if(temp[0] == '#') continue;
        strOff = getFirstString(temp, temp2);


        if(!strcmp(temp2, "v")) vert++;
        else if(!strcmp(temp2, "f")) poly++;
        else if(!strcmp(temp2, "vt")) tex++;
        else if(!mtlFound)
        {
            if(!strcmp(temp2, "mtllib"))
            {
                mtlLib = temp+strOff+1;
                mtlFound = 1;
            }
        }
    }
    printf("vert = %d\ntex = %d\npoly = %d\n", vert, tex, poly);
    vertData = new vec3f[vert];
    texData = new vec2f[tex];
    polygones = new triangle[poly];

    puts(path.c_str());
    path.erase(path.rfind('/')+1);
    puts(path.c_str());
    mtlLib[mtlLib.size()-1] = 0;

    mathmtl(path,mtlLib);

    vert = tex = poly = 0;
    int mat = -1, mp = 0;
    fseek(objFile, 0, 0);

    while(!feof(objFile))
    {
        fgets(temp, 1024, objFile);
        if(temp[0] == '#') continue;
        strOff = getFirstString(temp, temp2);
        if(!strcmp(temp2, "v"))
        {
            sscanf(temp+strOff, "%f %f %f", &vertData[vert].x, &vertData[vert].y, &vertData[vert].z);
            vert++;
        }
        else if(!strcmp(temp2, "f"))
        {

            sscanf(temp+strOff+1, "%d/%d %d/%d %d/%d",
                   &polygones[poly].v[0], &polygones[poly].t[0],
                   &polygones[poly].v[1], &polygones[poly].t[1],
                   &polygones[poly].v[2], &polygones[poly].t[2]);

            poly++;
            mp++;
        }
        else if(!strcmp(temp2, "vt"))
        {
            sscanf(temp+strOff, "%f %f", &texData[tex].x, &texData[tex].y);
            tex++;
        }
        else if(!strcmp(temp2, "usemtl"))
        {
            mat = findmtl(temp+strOff+1);

            printf("%d ", mat);
            materials[mat].offset = poly;
            mp = 0;
        }
        if(mat >= 0) materials[mat].count = mp;
    }

    fclose(objFile);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex), (GLuint*)(3*sizeof(float)));
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), 0);
    glBindVertexArray(0);



    vertex *data = new vertex[poly*3];
    int index, tindex;
    for(int k = 0; k < matCount; k++)
        for(int i = 0; i < materials[k].count; i++)
            for(int j = 0; j < 3; j++)
                {
                    index = polygones[materials[k].offset+i].v[j]-1;
                    tindex = polygones[materials[k].offset+i].t[j]-1;
                    data[(materials[k].offset+i)*3+j].tex = texData[tindex];
                    data[(materials[k].offset+i)*3+j].vert = vertData[index];
                }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, poly*3*sizeof(vertex), data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete [] data;
    delete [] vertData;
    delete [] texData;
    delete [] polygones;
    return 1;
}

bool model::mathmtl(string path, string name)
{
    name = path+name;
    printf("matName = %s\n", name.c_str());
    printf("matPath = %s\n", path.c_str());
    FILE *mtl = fopen(name.c_str(), "rt");
    if(!mtl) return 0;
    int mat = 0;
    char temp[1024], temp2[32];

    while(!feof(mtl))
    {
        fgets(temp, 1024, mtl);
        if(temp[0] == '#') continue;
        getFirstString(temp, temp2);
        if(!strcmp(temp2, "newmtl")) mat++;
    }

    fseek(mtl, 0, 0);

    materials = new material[mat];
    matCount = mat;
    mat = -1;
    string texPath;
    while(!feof(mtl))
    {
        fgets(temp, 1024, mtl);
        if(temp[0] == '#') continue;
        int strOff = getFirstString(temp, temp2);
        if(!strcmp(temp2, "newmtl"))
        {
            mat++;
            materials[mat].name = temp+strOff+1;
        }
        else if(!strcmp(temp2, "map_Kd"))
        {
            texPath = temp+strOff+1;
            texPath[texPath.size()-1] = 0;
            texPath = path+texPath;
            loadTexture(texPath.c_str(), &materials[mat].tex, 3);
        }
    }

    fclose(mtl);
    return 1;
}

void decodeIndex(const char *str, int *v, int *t, int *n)
{
    int length = 0;
    while(true)
    {
        if(str[length] == ' '
        || str[length] == 0) break;
        length++;
    }
    char temp[3][8];
    for(int i = 0; i < 3; i++) temp[i][0] = 0;
    int cell = 0;
    int s = 0;
    for(int i = 0; i < length; i++)
    {
        if(str[i] == '/')
        {
            s = 0;
            cell++;
            continue;
        }

        temp[cell][s] = str[i];
        s++;
    }

    *v = atoi(temp[0]);
    if(cell < 1) return;
    *t = atoi(temp[1]);
    if(cell < 2) return;
    *n = atoi(temp[2]);
}

void model::draw()
{
    glEnable(GL_TEXTURE_2D);
    glBindVertexArray(VAO);
        for(int i = 0; i < matCount; i++)
        {
            glBindTexture(GL_TEXTURE_2D, materials[i].tex);
            glDrawArrays(GL_TRIANGLES, materials[i].offset*3, materials[i].count*3);
        }
    glBindVertexArray(0);
}

int model::findmtl(string name)
{
    puts(name.c_str());
    for(int i = 0; i < matCount; i++)
        if(materials[i].name == name) return i;
    return -1;
}
