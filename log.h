#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

namespace _log{
    void init();
    void close();
    void out(const char*, bool = true);
    void out(const unsigned char*, bool = true);
}

#endif // LOG_H_INCLUDED
