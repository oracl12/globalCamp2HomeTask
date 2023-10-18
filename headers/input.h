#pragma once

#include <windows.h>
#include <windowsx.h>

class Input
{
public:
	struct Position
	{
		int x, y;
	};

	static Position &getMousePosition(LPARAM lParam);
};