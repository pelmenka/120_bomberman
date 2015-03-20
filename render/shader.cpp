#include <GL/glew.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>

#include "shader.h"
#include "../log.h"

shader::shader()
{
    shaders[0] = 0;
    shaders[1] = 0;
    program = -1;
}

shader::~shader()
{
    if(program == -1) return;
    unbind();
    glDetachShader(program, shaders[0]);
    glDetachShader(program, shaders[1]);
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);
    glDeleteProgram(program);
}

bool shader::addShader(bool shad, const char* path)
{
    int types[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    if(shaders[shad]) glDeleteShader(shaders[shad]);
    FILE *src = fopen(path, "rt");
    if(!src) return 0;
    fseek(src, 0, 2);
    int lenth = ftell(src);
    assert(lenth);
    fseek(src, 0, 0);
    char *source = new char[lenth];
    memset(source, 0, lenth);
    fread(source, 1, lenth, src);
    fclose(src);
    shaders[shad] = glCreateShader(types[shad]);
    const char *s = source;

    glShaderSource(shaders[shad], 1, &s, 0);
    _log::out("compiling shader -> ", 0);
    _log::out(path);
    glCompileShader(shaders[shad]);
    delete [] source;
    checkErrors(shad);
    return 1;
}

void shader::compile()
{
    program = glCreateProgram();
    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);
    glLinkProgram(program);
    int lenth;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenth);
    if(lenth)
    {
        char *mess = new char[lenth];
        glGetProgramInfoLog(program, lenth, 0, mess);
        _log::out(mess);
        delete [] mess;
    }
}

bool shader::load(const char *vert, const char *frag)
{
    if(addShader(0, vert) && addShader(1, frag))
    {
        compile();
        return 1;
    }
    return 0;
}

void shader::bind()
{
    glUseProgram(program);
}

void shader::unbind()
{
    glUseProgram(0);
}

void shader::checkErrors(GLuint i)
{
    int log = 0;
    glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &log);
    if(!log)
    {
        glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &log);
        if(i) _log::out("fragment shader:");
        else _log::out("vertex shader:");
        char *mess = new char[log];
        glGetShaderInfoLog(shaders[i], log, 0, mess);
        _log::out(mess);
        delete [] mess;
    }
}
