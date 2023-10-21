#pragma once


#include <windows.h>
#include <iostream>

class Debug 
{
public:
    Debug(){
        AllocConsole();
	    freopen("conin$","r",stdin);
	    freopen("conout$","w",stdout);
	    freopen("conout$","w",stderr);
        setvbuf(stdout, NULL, _IONBF, 0);
    }

	Debug(const Debug &) = delete;

	Debug &operator=(const Debug &) = delete;

    ~Debug(){
	    FreeConsole();
    }

};