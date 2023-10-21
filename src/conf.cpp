#include "../headers/conf.h"

int Conf::port = 5555;
Conf::GameMode Conf::gameMode = Conf::GameMode::BOT;
bool Conf::host = false;
bool Conf::debug = false;
std::wstring Conf::windowTitle = L"Warship Battle";