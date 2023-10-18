#pragma once

#ifdef _WIN32
	#ifndef UNICODE
		#define UNICODE
		#define UNICODE_WAS_UNDEFINED
	#endif

	#include <windows.h>
	#include <string>

	#ifdef UNICODE_WAS_UNDEFINED
		#undef UNICODE
	#endif
#else
	#include <string>
#endif

#define TITLE L"Warship Battle"

#include <functional>

#include "../headers/renderer.h"

class Game
{
	friend LRESULT CALLBACK WindowCallback(
		HWND windowHandle,
		UINT message,
		WPARAM wParam,
		LPARAM lParam);

private:
	HINSTANCE hInstance;
	HWND windowHandle = 0;
	bool running = false;
	char winner; // y - you, e - enemy

	int windowWidth = Renderer::width;
	int windowHeight = Renderer::height;

	std::function<void(float delta)> update;

public:
	Game();

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	~Game() {}

	inline static Game &getInstance()
	{
		static Game game;
		return game;
	}

	inline static void start()
	{
		getInstance().startWindow();
	}

	inline static void setGameUpdate(const std::function<void(float delta)> &update) { getInstance().update = update; }

	inline static std::wstring getWindowTitle() { return getInstance().windowTitle; }
	inline static int getWindowWidth() { return getInstance().windowWidth; }
	inline static int getWindowHeight() { return getInstance().windowHeight; }
	inline static char getWinner() { return getInstance().winner; }

	static bool isEndOfGame();
	static bool isShipPlacementFinished();
	// static void fillPrivateMatrix();

	static const int matrixS = 10;
	std::wstring windowTitle = TITLE;
private:
	void startWindow();

	inline static void setWinner(char win){
		if (win == 'e' || win == 'y')
			getInstance().winner = win;
	}
};