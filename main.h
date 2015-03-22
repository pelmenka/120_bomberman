#ifndef MAIN_H
#define MAIN_H

#include "sound.h"

extern soundEngine soundBox;
extern hero actor;
extern level field;

bool getKey(int, bool = 0);

void reset();

#endif // MAIN_H
