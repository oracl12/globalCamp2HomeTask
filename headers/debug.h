#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif

#include <iostream>

class Debug 
{
public:
    Debug(){
        #ifdef _WIN32
        AllocConsole();
	    freopen("conin$","r",stdin);
	    freopen("conout$","w",stdout);
	    freopen("conout$","w",stderr);
        setvbuf(stdout, NULL, _IONBF, 0);
        #endif
    }

	Debug(const Debug &) = delete;

	Debug &operator=(const Debug &) = delete;

    ~Debug(){
        #ifdef _WIN32
	    FreeConsole();
        #endif
    }

    // currently not in need
    void flushOutput() {
        fflush(stdout);
    }
};