#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "log.h"

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

template<class t>
struct vec2
{
    t x, y;
};

template<class t>
struct vec3
{
    t x, y, z;
    vec3<t>& operator+=(vec3<t> a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
    }
};

template<class t>
struct vec4
{
    t x, y, z, w;
};

typedef vec2<char> vec2c;
typedef vec2<int> vec2i;
typedef vec2<float> vec2f;

typedef vec2<unsigned char> vec2b;
typedef vec2<unsigned int> vec2u;
typedef vec2<double> vec2d;

typedef vec3<char> vec3c;
typedef vec3<int> vec3i;
typedef vec3<float> vec3f;

typedef vec3<unsigned char> vec3b;
typedef vec3<unsigned int> vec3u;
typedef vec3<double> vec3d;

typedef vec4<char> vec4c;
typedef vec4<int> vec4i;
typedef vec4<float> vec4f;

typedef vec4<unsigned char> vec4b;
typedef vec4<unsigned int> vec4u;
typedef vec4<double> vec4d;


#endif // COMMON_H_INCLUDED
