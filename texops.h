#ifndef TEXOPS_H_INCLUDED
#define TEXOPS_H_INCLUDED

#include "common.h"
#include <string>

vec2<long> loadTexture(const char*, GLuint*, int = 3);
vec2<long> loadTexture(std::string, GLuint*, int = 3);
void cleanTextures();

#endif // TEXOPS_H_INCLUDED
