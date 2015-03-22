#include "sound.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

soundEngine::soundEngine()
{
    ok = 1;
    engine = createIrrKlangDevice();
    if(!engine) ok = 0;
    if(!ok) return;
}

soundEngine::~soundEngine()
{
    if(!ok) return;
    engine->drop();
}

void soundEngine::playSound(const char *songName)
{
    if(!songName || !ok) return;
    engine->play2D(songName);
}

void soundEngine::setVolume(float volume)
{
    if(engine->getSoundVolume() != volume)
        engine->setSoundVolume(volume);
}

float soundEngine::getVolume()
{
    return engine->getSoundVolume();
}

