#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <windows.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#include "headers/game.h"
#include "headers/renderer.h"
#include "headers/input.h"
#include "headers/ship.h"
#include "headers/bot.h"
#include "headers/player.h"
#include "headers/debug.h"
#include "headers/conf.h"
#include "headers/command_line.h"
#include "headers/network/client.h"
#include "headers/network/server.h"
#include "headers/network/socket_utils.h"

// this is possible shared variables
bool ready = false;

extern bool playerStep;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	CommandLineHandler commandLineHandler;

	Debug* debug;
	Server* server;
	Client* client;

	if (Conf::debug) {
		debug = new Debug;
	}

	if (Conf::gameMode == Conf::GameMode::BOT) {
		std::clog << "GAMEMODE - BOT" << std::endl;
		playerStep = true;
		Bot::generateShipsOnMatrix();
	} else {
		std::clog << "GAMEMODE - NETWORK" << std::endl;
		std::clog << "PORT     - "<< Conf::port << std::endl;

		if (Conf::host)
		{
			playerStep = true;
			server = new Server(Conf::port);
			std::cout << "SERVER is ruNNING" << std::endl;
		} else {
			playerStep = false;
			client = new Client(Conf::port);
			std::cout << "CLIENT is ruNNING" << std::endl;
		}
	}

	std::vector<Ship> &ships = ShipHandler::getShips();
	ShipHandler::fillUpShips();

	Game::setGameUpdate([&](float delta)
						{
		wchar_t charBuffer[256]; 

		static int frames = 0;
		static float timePassed = 0.0f;

		frames++;
		timePassed += delta;

		if (timePassed >= 1.0f)
		{
			swprintf(charBuffer, 256, L"FPS: %d\n", frames);

			timePassed -= 1.0f;
			frames = 0;
			std::wcout << charBuffer << std::endl;
		}

		// left table
		for (int line = 0; line < Conf::matrixS; line++) {
			for (int row = 0; row < Conf::matrixS; row++) {
				RGBColor color;
				switch ((ready ? Player::getPublicMatrix() : Player::getPrivateMatrix())[line][row]) {
					case -1: // sector is cleared
						color = { 12, 21, 12 };
						break;
					case 1: // sector where ship is hitten
						color = { 255, 5, 5 };
						break;
					case 2: // ship of yours
						color = { 255, 192, 203 };
						break;
					default: // hidden tile
						color = { 128, 128, 128 };
				}

				Renderer::FillRectangleWithBorder(
					{ Conf::start_l_x + row * Conf::size, 20 + line * Conf::size,
					 Conf::size, Conf::size },  color);
			}
		}

		// button ready
		Renderer::FillRectangleWithBorder({ 540, 640, 200, 60 }, { 102, 51, 153 });

		// just line between two tables
		Renderer::FillRectangle( { 1280 / 2 - 2, 0, 4, 600 }, { 255, 5, 5 });

		// right table
		for (int line = 0; line < Conf::matrixS; line++) {
			for (int row = 0; row < Conf::matrixS; row++) {
				RGBColor color;
				switch (Enemy::getPublicMatrix()[line][row]) {
					case -1: // sector is cleared
						color = { 12, 21, 12 };
						break;
					case 1: // sector where ship is hitten
						color = { 255, 5, 5 };
						break;
					case 2: // ship of yours
						color = { 255, 192, 203 };
						break;
					default: // hidden tile
						color = { 128, 128, 128 };
				} 

				Renderer::FillRectangleWithBorder(
					{ Conf::start_r_x + row * Conf::size, 20 + line * Conf::size,
					 Conf::size, Conf::size }, color);
			}
		}

		// bottom bar
		for (auto& ship : ships) {
			if (ship.avaible) {
				for (auto& rect : ship.rects) {
					Renderer::FillRectangle(rect, { 0, 255, 0 });
				}
			}
		} });
	Renderer::SetClearColor({200, 120, 45});

	Game::start();

	if (debug != nullptr) 
		delete debug;
	
	return 0;
}