#ifdef _WIN32

#include "../music.h"
#include <windows.h>
#include <stdio.h>

void musicEngine::findMusic(const char *path)
{
    char temp[256];

	WIN32_FIND_DATA findFileData;
	HANDLE hFile;

    sprintf(temp, "%s/*.ogg", path);
    hFile=FindFirstFile(temp, &findFileData);
    if (hFile!=INVALID_HANDLE_VALUE)
    {
        do
        {
            sprintf(temp, "%s/%s", path, findFileData.cFileName);
            songs.push_back(temp);
        }
        while(FindNextFile(hFile,&findFileData) != 0);
        FindClose(hFile);
    }
}

#endif // WIN32
