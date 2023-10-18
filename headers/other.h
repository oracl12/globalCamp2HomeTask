#pragma once

#include <unistd.h>

inline void Sleep(int seconds)
{
#ifdef _WIN32
    Sleep(seconds * 1000);
#else
    sleep(seconds);
#endif
}