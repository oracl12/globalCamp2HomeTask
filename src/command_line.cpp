#include "../headers/command_line.h"

#ifdef _WIN32
CommandLineHandler::CommandLineHandler()
{

    szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

    if (szArgList == NULL)
    {
        MessageBoxW(NULL, L"Unable to parse command line", L"Error", MB_OK);
        exit(10);
    }

    for (int i = 0; i < argCount; i++)
    {
        if (wmemcmp(szArgList[i], L"-debug", wcslen(L"-debug")) == 0) // remove here wcslen
        {
            debug = true;
        }

        if (wmemcmp(szArgList[i], L"-host", wcslen(L"-host")) == 0) // remove here wcslen
        {
            host = true;
        }

        if (wmemcmp(szArgList[i], L"--gamemode=network", wcslen(L"--gamemode=network")) == 0) // remove here wcslen
        {
            gameMode = 'n';
        }

        if (wcsncmp(szArgList[i], L"--port=", wcslen(L"--port=")) == 0)
        {
            std::wstring portValue = szArgList[i] + wcslen(L"--port=");

            if (!portValue.empty())
            {
                port = std::stoi(portValue);
                std::wcout << L"Port number: " << port << std::endl;
            }
            else
            {
                std::wcerr << L"Port number not specified." << std::endl;
            }
        }

        if (wmemcmp(szArgList[i], L"--gamemode=bot", wcslen(L"--gamemode=bot")) == 0) // remove here wcslen
        {
            gameMode = 'b';
        }
    }
};
#else
    CommandLineHandler::CommandLineHandler(int argc, char* argv[])
    {
    for (int i = 0; i < argc; i++)
    {
        if (memcmp(argv[i], "-debug", 6) == 0) // remove here wcslen
        {
            debug = true;
        }

        if (memcmp(argv[i], "-host", 5) == 0)
        {
            host = true;
        }

        if (memcmp(argv[i], "--gamemode=network", 19) == 0)
        {
            gameMode = 'n';
        }

        if (strncmp(argv[i], "--port=", 8) == 0)
        {
            std::string portValue = argv[i] + 8;

            if (!portValue.empty())
            {
                port = std::stoi(portValue);
                std::cout << "Port number: " << port << std::endl;
            }
            else
            {
                std::cerr << "Port number not specified." << std::endl;
            }
        }

        if (memcmp(argv[i], "--gamemode=bot", 15) == 0)
        {
            gameMode = 'b';
        }
    }
    }
#endif
CommandLineHandler::~CommandLineHandler()
{
    #ifdef _WIN32
        LocalFree(szArgList);
    #endif
};
