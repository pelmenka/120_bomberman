#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

class shader
{
public:
    unsigned int shaders[2], program;
    void checkErrors(unsigned i);
    shader();
    ~shader();

    void bind();
    void unbind();

    bool load(const char*, const char*);

    bool addShader(bool, const char*);
    void compile();
};

#endif // SHADER_H_INCLUDED
