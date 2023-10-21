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

	int windowWidth = Renderer::width;
	int windowHeight = Renderer::height;

	std::function<void(float delta)> update;

public:
	Game();

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	~Game() {}

	enum class Winner
	{
		PLAYER,
		ENEMY,
		NONE
	};

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

	inline static int getWindowWidth() { return getInstance().windowWidth; }
	inline static int getWindowHeight() { return getInstance().windowHeight; }

	static Game::Winner isEndOfGame();
	static bool isShipPlacementFinished();
private:
	void startWindow();
};