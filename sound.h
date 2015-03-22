#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <irrKlang.h>

using namespace irrklang;

class soundEngine
{
public:

    bool ok;
    ISoundEngine *engine;

    void setVolume(float);
    float getVolume();

    soundEngine();
    ~soundEngine();
    void playSound(const char*);
};

#endif // SOUND_H_INCLUDED
