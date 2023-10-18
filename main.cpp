#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#ifdef _WIN32
	#include <windows.h>
#endif

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
#include "headers/command_line.h"
#include "headers/other.h"
#include "headers/network/client.h"
#include "headers/network/server.h"
#include "headers/network/socket_utils.h"

const int MATRIX_S = 10;
int size = 50;
int start_l_x = 75;
int start_r_x = 740 + 6;
char gameMode;
bool ready = false;
bool host = false;

#ifdef _WIN32
extern HDC device_context;
#endif
extern bool playerStep;

void fillPrivateMatrix()
{
	for (int i = 0; i < MATRIX_S; ++i)
	{
		for (int j = 0; j < MATRIX_S; ++j)
		{
			Player::getPrivateMatrix()[i][j] = -1;
			if (gameMode == 'b')
				Bot::getPublicMatrix()[i][j] = -1;
		}
	}
}

#ifdef _WIN32
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{

	#ifdef _WIN32
		CommandLineHandler commandLineHandler;
	#else
		CommandLineHandler commandLineHandler(argc, argv);
	#endif

	Debug* debug;
	Server* server;
	Client* client;

	if(commandLineHandler.getDebugStatus()) {
		debug = new Debug;
	}

	if (commandLineHandler.getGameMode() == 'b') {
		std::clog << "GAMEMODE - BOT" << std::endl;
		playerStep = true;
		gameMode = 'b';
		Bot::generateShipsOnMatrix();
	} else if (commandLineHandler.getGameMode() == 'n') {
		std::clog << "GAMEMODE - NETWORK" << std::endl;
		int port = commandLineHandler.getPort();
		std::clog << "PORT     - "<< port << std::endl;
		gameMode = 'n';

		if (commandLineHandler.isHost())
		{
			host = true;
			playerStep = true;
			server = new Server(port);
			std::cout << "SERVER is ruNNING" << std::endl;
		} else {
			playerStep = false;
			client = new Client(port);
			std::cout << "CLIENT is ruNNING" << std::endl;
		}
	} else {
		std::cerr << "GAMEMODE - UNDEFINED" << std::endl;
		std::cerr << "EXItiNG" << std::endl;
		Sleep(1);
		return 0;
	}

	std::vector<Ship> &ships = ShipHandler::getShips();
	ShipHandler::fillUpShips();

#ifdef _WIN32
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
			// std::wcout << charBuffer << std::endl;
		}

		// left table
		for (int line = 0; line < MATRIX_S; line++) {
			for (int row = 0; row < MATRIX_S; row++) {
				RGBColor color;
				switch ((ready ? Player::getPublicMatrix() : Player::getPrivateMatrix())[line][row]) {
					case -1: // sector is clear
						color = { 12, 21, 12 };
						break;
					case 1: // sector where ship is hitten
						color = { 255, 5, 5 };
						break;
					case 2: // ship of yours
						color = { 255, 192, 203 };
						break;
					default: // just tile
						color = { 128, 128, 128 };
				}

				Renderer::FillRectangleWithBorder(
					{ start_l_x + row * size, 20 + line * size,
					 size, size },  color);
			}
		}

		// button ready
		Renderer::FillRectangleWithBorder({ 540, 640, 200, 60 }, { 102, 51, 153 });

		// just line between two tables
		Renderer::FillRectangle( { 1280 / 2 - 2, 0, 4, 600 }, { 255, 5, 5 });

		// right table
		for (int line = 0; line < MATRIX_S; line++) {
			for (int row = 0; row < MATRIX_S; row++) {
				RGBColor color;
				switch (Enemy::getPublicMatrix()[line][row]) {
					case -1: // sector is clear
						color = { 12, 21, 12 };
						break;
					case 1: // sector where ship is hitten
						color = { 255, 5, 5 };
						break;
					case 2: // ship of yours
						color = { 255, 192, 203 };
						break;
					default: // just tile
						color = { 128, 128, 128 };
				} 

				Renderer::FillRectangleWithBorder(
					{ start_r_x + row * size, 20 + line * size,
					 size, size }, color);
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
	#endif
	if (debug != nullptr) 
		delete debug;
	
	return 0;
}