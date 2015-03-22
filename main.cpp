#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "render/render.h"
#include "level.h"
#include "hero.h"

GLFWwindow *window;

void glfwError(int, const char*);
void writeInfo();

double scroll;
void drawShadow(int x, int y);
void updateCamera(vec3f*);

hero actor;
level field;

void checkScroll(GLFWwindow*, double, double s)
{
    scroll = s;
}

void checkTex();

void putTex(int, int, bool);

int main()
{

    _log::init();
    glfwSetErrorCallback(glfwError);
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1280, 800, "pelmenka", 0, 0);

    glfwSetScrollCallback(window, checkScroll);

    glfwMakeContextCurrent(window);
    glewInit();
    srand(time(0));
    writeInfo();

    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gluPerspective(60, 1280.0/800.0, 0.1, 100);
    glEnable(GL_TEXTURE_2D);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    render::init();

    vec2<float> off = {0, 0};
    vec3f cam = {0, 0, 0};
    glClearColor(0.5, 0.7, 0.9, 1);


    actor.init();

    //emitter test(1000);
    //test.nospawn = 1;
    field.init();
    int i = 0;
    vec2f epos;
    float time = 0;
    char temp[32];

    field.generate();

    while(!glfwWindowShouldClose(window))
    {

        updateCamera(&cam);
        actor.update(cam.x);
        field.updateArmy();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(glfwGetKey(window, GLFW_KEY_Q))
            glPolygonMode(GL_FRONT, GL_LINE);
        else
            glPolygonMode(GL_FRONT, GL_FILL);

        off = actor.getPos();
        glPushMatrix();
        glTranslatef(0, 0, cam.z-5);
        glRotatef(cam.y, 1, 0, 0);
        glRotatef(cam.x, 0, 0, 1);
        glTranslatef(-off.x-0.5, -off.y-0.5, 0);

        field.draw();
        actor.draw();
        field.drawArmy();
        render::game::drawParticles();
        glPopMatrix();
        glfwSwapBuffers(window);
        glfwPollEvents();


        sprintf(temp, "%f", 1.0/(glfwGetTime()-time));
        glfwSetWindowTitle(window, temp);
        time = glfwGetTime();
    }


    //cleanTextures();
    glfwTerminate();
    _log::close();
}

void glfwError(int type, const char* error)
{
    bool fatal = 0;
    switch(type)
    {
    case GLFW_NOT_INITIALIZED:
    case GLFW_NO_CURRENT_CONTEXT:
    case GLFW_OUT_OF_MEMORY:
    case GLFW_API_UNAVAILABLE:
    case GLFW_VERSION_UNAVAILABLE:
    case GLFW_PLATFORM_ERROR:
        _log::out("[ERROR]", 0);
        fatal = 1;
        break;
    default:
        _log::out("[WARNING]", 0);
        break;
    }
    _log::out(error);
    if(fatal) abort();
}

void updateCamera(vec3f *rot)
{
    static vec2d scur = {0, 0};
    vec2d cur;
    static bool pressed = 0;
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
    {
        glfwGetCursorPos(window, &cur.x, &cur.y);
        if(!pressed)
        {
            scur = cur;
            pressed = 1;
        }

        rot->x += (cur.x-scur.x)/2.0;
        rot->y += (cur.y-scur.y)/2.0;
        if(rot->y > 0) rot->y = 0;
        if(rot->y < -60) rot->y = -60;
        if(rot->x < 0) rot->x += 360;
        scur = cur;
    }
    else pressed = 0;

    if(scroll != 0)
    {
        rot->z += scroll/5.0;
        if(rot->z < -4) rot->z = -4;
        else if(rot->z > 2) rot->z = 2;
        scroll = 0;
    }
}

bool getKey(int key, bool force)
{
    static int oldkey = 0;
    bool result;

    if(key < 10)
        result = glfwGetMouseButton(window, key);
    result = glfwGetKey(window, key);

    return result;
}

void writeInfo()
{
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    char hardVersion[256];
    sprintf(hardVersion, "Detected GL version: %d.%d.%d", major, minor, rev);
    _log::out(hardVersion);
    _log::out("Vendor: ", 0);
    _log::out(glGetString(GL_VENDOR));
    _log::out("Version: ", 0);
    _log::out(glGetString(GL_VERSION));
    _log::out("Renderer: ", 0);
    _log::out(glGetString(GL_RENDERER));
}
