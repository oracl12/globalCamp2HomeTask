#pragma once

#ifdef _WIN32
    #ifndef UNICODE
        #define UNICODE
        #define UNICODE_WAS_UNDEFINED
    #endif

    #include <windows.h>

    #ifdef UNICODE_WAS_UNDEFINED
    #undef UNICODE
    #endif

    #include <string>
#else
    #include <string.h>
#endif

#include <iostream>


class CommandLineHandler
{
public:
    CommandLineHandler();

    CommandLineHandler(int argc, char* argv[]);

	CommandLineHandler(const CommandLineHandler &) = delete;

	CommandLineHandler &operator=(const CommandLineHandler &) = delete;

    const inline char getGameMode()
    {
        return gameMode;
    }

    const inline bool getDebugStatus()
    {
        return debug;
    }

    const inline int getPort()
    {
        return port;
    }

    const inline int isHost()
    {
        return host;
    }

    const inline int getPeriodL()
    {
        return periodL;
    }

    const inline std::wstring getParamsL()
    {
        return paramsL;
    }

    ~CommandLineHandler();

private:
    char gameMode;
    int port;
    bool debug = false;
    #ifdef _WIN32
        LPWSTR *szArgList;
    #endif
    int argCount;
    bool host;
    int periodL = 1;
    std::wstring paramsL;
};