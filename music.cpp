#include "music.h"
#include "sound.h"
#include <stdio.h>
#include <stdlib.h>

musicEngine::musicEngine()
{
    findMusic("data/music");
    if(songs.size())
    currentPlaying = rand()%songs.size();
}


void musicEngine::update()
{
    if(!ok) return;
    setVolume(0.5);
    if(songs.size() <= 0) return;

    for(std::string i : songs)
        if(engine->isCurrentlyPlaying(i.c_str())) return;

    int temp = 0;

    if(songs.size() > 1)
    do
    {
        temp = rand()%songs.size();
    }while(temp == currentPlaying);

    currentPlaying = temp;
    playSound(songs[temp].c_str());
}
