#include "../headers/command_line.h"

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
        if (wmemcmp(szArgList[i], L"-debug", 6) == 0)
        {
            Conf::debug = true;
        }

        if (wmemcmp(szArgList[i], L"-host", 5) == 0)
        {
            Conf::host = true;
        }

        if (wmemcmp(szArgList[i], L"--gamemode=network", 19) == 0)
        {
            Conf::gameMode = Conf::GameMode::NETWORK;
        }

        if (wcsncmp(szArgList[i], L"--port=", 8) == 0)
        {
            std::wstring portValue = szArgList[i] + 8;

            if (!portValue.empty())
            {
                Conf::port = std::stoi(portValue);
                std::wcout << L"Port number: " << Conf::port << std::endl;
            }
            else
            {
                std::wcerr << L"Port number not specified." << std::endl;
            }
        }

        if (wmemcmp(szArgList[i], L"--gamemode=bot", 15) == 0)
        {
            Conf::gameMode = Conf::GameMode::BOT;
        }
    }
};

CommandLineHandler::~CommandLineHandler()
{
    LocalFree(szArgList);
};
