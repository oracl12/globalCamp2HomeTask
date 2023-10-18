#pragma once

#ifdef _WIN32
	#include <windows.h>
	#include <windowsx.h>
#else

#endif

class Input
{
public:
	struct Position
	{
		int x, y;
	};

	static Position &getMousePosition(LPARAM lParam);
};