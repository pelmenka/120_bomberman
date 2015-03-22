#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include <stdio.h>
#include <list>
#include "texops.h"

GLFWimage loadImage(const char*);

std::list<uint*> textures;

vec2<long> loadTexture(std::string path, GLuint *texture, int flag)
{
    loadTexture(path.c_str(), texture, flag);
}

vec2<long> loadTexture(const char *path, GLuint *texture, int flag)
{
    GLFWimage img;
    vec2<long> size = {-1, -1};

    _log::out("loading texture -> ", 0);
    _log::out(path, 0);

    img = loadImage(path);

    if(img.height == -1)
    {
        _log::out(" [FAIL]");
        return size;
    }

    size.x = img.width;
    size.y = img.height;


    if(!glIsTexture(*texture))
        glGenTextures(1, texture);

    glBindTexture(GL_TEXTURE_2D, *texture);
    if(flag&0x4) gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)img.pixels);
    else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)img.pixels);

    if(flag&0x1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(flag&0x2) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



    delete [] img.pixels;

    textures.push_back(texture);

    _log::out(" [OK]");

    return size;
}

GLFWimage loadImage(const char *path)
{
    GLFWimage temp = {-1, -1, 0};
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path,0);
	FIBITMAP* image = FreeImage_Load(format, path);
	if(!image && image == NULL) return temp;
	image = FreeImage_ConvertTo32Bits(image);
	temp.width = FreeImage_GetWidth(image);
	temp.height = FreeImage_GetHeight(image);
	temp.pixels = new GLubyte[4*temp.width*temp.height];
	char* pixeles = (char*)FreeImage_GetBits(image);
	for(int j = 0; j < temp.width*temp.height; j++)
	{
		temp.pixels[j*4+0] = pixeles[j*4+2];
		temp.pixels[j*4+1] = pixeles[j*4+1];
		temp.pixels[j*4+2] = pixeles[j*4+0];
		temp.pixels[j*4+3] = pixeles[j*4+3];
	}
	FreeImage_Unload(image);
	return temp;
}

void cleanTextures()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    for(auto i: textures)
        glDeleteTextures(1, i);
}
