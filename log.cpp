#include <fstream>
#include "log.h"

using namespace std;

fstream logFile;

void _log::init()
{
    logFile.open("log.txt", ios::out);
}

void _log::close()
{
    logFile.close();
}

void _log::out(const char *msg, bool ret)
{
    logFile << msg;
    if(ret)
        logFile << endl;
}

void _log::out(const unsigned char *msg, bool ret)
{
    logFile << msg;
    if(ret)
        logFile << endl;
}
