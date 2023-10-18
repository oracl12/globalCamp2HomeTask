#pragma once

#include <windows.h>
#include <winuser.h>
#include <stdint.h>

#include "loadability.h"

struct RGBColor
{
	uint8_t red, green, blue;
};

struct Rect
{
	int x, y, width, height;
	bool hitten = false;
	struct {
        int x;
        int y;
    } placeOnMatrix = { -1, -1 };
};

class Renderer
{
	friend LRESULT CALLBACK WindowCallback(
		HWND windowHandle,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);

	friend class Game;

	struct BitmapBuffer
	{
		int width, height;
		BITMAPINFO info;
		void *memory;
		int pitch; // in bytes
	};

private:
	static const int bytes_per_pixel = 4;
	static const int width = 1280;
	static const int height = 720;

	HWND windowHandle = 0;
	BitmapBuffer buffer;
	RGBColor clearColor;

public:
	inline static void SetClearColor(const RGBColor &color) { getInstance().clearColor = color; }

	static void SetPixel(int x, int y, const RGBColor &color);

	static void FillRectangle(const Rect &rect, const RGBColor &color);

	static void FillRectangleWithBorder(const Rect &rect, const RGBColor &color, const RGBColor &borderColor = {0, 0, 0});

	static void DrawRectangle(const Rect &rect, const RGBColor &color);

private:
	Renderer()
	{
		buffer = {};
		clearColor = {255, 255, 255};
	}

	Renderer(const Renderer &) = delete;
	Renderer &operator=(const Renderer &) = delete;

	~Renderer() {}

	inline static Renderer &getInstance()
	{
		static Renderer renderer;
		return renderer;
	}

private:
	inline static void setWindowHandle(HWND _windowHandle) { getInstance().windowHandle = _windowHandle; }

	static void allocBuffer();

	static void copyBufferToWindow(HDC deviceContext, bool yourStep);

	static void clear();
};