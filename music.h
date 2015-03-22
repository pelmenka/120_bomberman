#ifndef MUSIC_INCLUDED
#define MUSIC_INCLUDED

#include "sound.h"

#include <vector>
#include <string>

class musicEngine: public soundEngine
{
    void readMusicList();
public:
    std::vector<std::string> songs;
    int numSongs;
    float volume;
    int lenth, currentPlaying;

    musicEngine();
    void update();
    void findMusic(const char*);
};

#endif // MUSIC_H_INCLUDED
