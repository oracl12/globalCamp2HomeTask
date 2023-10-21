#pragma once

#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <windows.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#include <string>
#include <iostream>
#include "conf.h"

class CommandLineHandler
{
public:
    CommandLineHandler();

    CommandLineHandler(const CommandLineHandler &) = delete;

    CommandLineHandler &operator=(const CommandLineHandler &) = delete;

    ~CommandLineHandler();

private:
    LPWSTR *szArgList;
    int argCount;
};