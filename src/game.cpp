#include "../headers/game.h"
#include "../headers/input.h"
#include "../headers/renderer.h"
#include "../headers/ship.h"
#include "../headers/player.h"
#include "../headers/enemy.h"
#include "../headers/bot.h"

#include <iostream>
#include <mutex>

extern int start_l_x;
extern int start_r_x;
extern int size;
extern char gameMode;
extern bool ready;
extern bool host;

std::mutex exchangeMutex;
bool forceExhangeMatrix = false;
bool isDragging = false;
bool playerStep;
Ship* currentlyMovingShip;

Game::Game() {}

LRESULT CALLBACK WindowCallback(
	HWND windowHandle,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
	case WM_COMMAND:{
		HCURSOR hCursor = LoadCursor(NULL, IDC_UPARROW);
        SetCursor(hCursor);
	} break;
	case WM_CLOSE:
	{
		Game::getInstance().running = false;
	}
	break;
	case WM_DESTROY:
	{
		Game::getInstance().running = false;
	}
	break;
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	{
		if (currentlyMovingShip != nullptr) {
			std::cout << "WE ARE IN SWITCH" << std::endl;
			switch (wParam) {
        		case VK_LEFT:
					// maybe repaint also in need
					Player::changeDirection(*currentlyMovingShip, 'h');
                	break;
            	case VK_RIGHT:
					Player::changeDirection(*currentlyMovingShip, 'h');
                	break;
            	case VK_UP:
					Player::changeDirection(*currentlyMovingShip, 'v');
                	break;
            	case VK_DOWN:
					Player::changeDirection(*currentlyMovingShip, 'v');
                	break;
            }
		}
	} break;
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	{
		auto currentPos = Input::getMousePosition(lParam);
		
		// Check if we are in range of ready button { 540, 640, 200, 60 }
		if (currentPos.x < 740 && currentPos.x > 540 && currentPos.y > 640 && currentPos.y < 700 ){
			std::cout << "CLicking on READY BUTTON" << std::endl;
			ready = true;
		}

		Ship* shipAtMouse = ShipHandler::findShipByMouse(currentPos.x, currentPos.y);

		if (shipAtMouse == nullptr) {
			std::cout << "There no ship at tile" << std::endl;
		} else {
			isDragging = true;
			break;
		}

		if (currentPos.x < start_r_x || currentPos.x > (start_r_x + 500) || currentPos.y < 20 || currentPos.y > 520 ){
			std::cout << "BREAK CAUSE OF siZE of matrix" << std::endl;
			break;
		}
		
		if (!playerStep){
			std::cout << "BREAK CAUSE OF STEP" << std::endl;
			break;
		}
		
		// if (!Game::isShipPlacementFinished()){
			// 	MessageBoxA(NULL, "Ship placement is not finished", "Placement error", MB_OK | MB_ICONINFORMATION);
			// 	break;
		// }

		int x = (int)((currentPos.x - start_r_x) / 50);
		int y = (int)((currentPos.y - 20) / 50);

		// if we shoot correctly, continue shooting
		if (Player::makeShot(x, y)){
			break;
		}

		if (Game::isEndOfGame()){
			MessageBoxA(NULL, Game::getWinner() == 'y' ? "You are winner" : "Enemy wins", "Game ends", MB_OK | MB_ICONINFORMATION);
			exit(1);
		}

		if (gameMode == 'b') {
			playerStep = false;
			Bot::entry_point();
		} else if (gameMode == 'n') {
			{
				std::unique_lock<std::mutex> lock(exchangeMutex);
				forceExhangeMatrix = true;
			}
		}
	} break;
	case WM_LBUTTONUP:
	{
		if (isDragging) {
			isDragging = false;
			auto currentPos = Input::getMousePosition(lParam);
		
			if (currentPos.x < start_l_x || currentPos.x > (start_l_x + 500) || currentPos.y < 20 || currentPos.y > 520 ){
				break;
			}

			int x = (int)((currentPos.x - start_l_x) / 50);
			int y = (int)((currentPos.y - 20) / 50);

			Ship* shipAtMouse = ShipHandler::findShipByMouse(currentPos.x, currentPos.y);

			if (shipAtMouse == nullptr) {
				std::cout << "There no ship at tile" << std::endl;
				break;
			}

			if (!shipAtMouse->avaible) {
				std::cout << "This ship is unavaible" << std::endl;
				break;
			}

			if (!Player::placeShip(x, y, *shipAtMouse)){
				std::cout << "UNABLE TO PLACE ship here" << std::endl;
				break;
			}

			std::cout << "PLACING ship successfully" << std::endl;
			shipAtMouse->avaible = false;
			break;
		}

		isDragging = false;
	} break;
	case WM_RBUTTONDOWN:{
		auto currentPos = Input::getMousePosition(lParam);

		if (currentPos.x < start_l_x || currentPos.x > (start_l_x + 500) || currentPos.y < 20 || currentPos.y > 520 ){
			break;
		}

		int x = (int)((currentPos.x - start_l_x) / 50);
		int y = (int)((currentPos.y - 20) / 50);

		Ship* shipAtMouse = ShipHandler::findShipByMouse(currentPos.x, currentPos.y);
		
		if (shipAtMouse == nullptr) {
			std::cout << "There no ship at tile" << std::endl;
			break;
		}

		if (shipAtMouse->avaible) {
			std::cout << "This ship is avaible. Cannot delete" << std::endl;
			break;
		}

		if (Player::removeShip(x, y, *shipAtMouse)) {
			std::cout << "Removed ship successfully" << std::endl;
		}
		std::cout << "There no ship on this tile" << std::endl;

	}break;
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEMOVE:
	{
		if (isDragging && (wParam & MK_LBUTTON))
		{
			auto currentPos = Input::getMousePosition(lParam);

			currentlyMovingShip = ShipHandler::findShipByMouse(currentPos.x, currentPos.y);

			if (currentlyMovingShip == nullptr)
				break;

			Player::changeShipPosition(currentPos.x, currentPos.y, currentlyMovingShip);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		HDC device_context = BeginPaint(windowHandle, &paint);

		Renderer::copyBufferToWindow(device_context, playerStep);

		EndPaint(windowHandle, &paint);
	}
	break;

	default:
		result = DefWindowProc(windowHandle, message, wParam, lParam);
	}

	return result;
}

bool Game::isEndOfGame(){
	bool flag = true; // all ships are fully hitten

	// first check your ships are all dead
	for (const Ship& ship : ShipHandler::getShips()){
		for (const Rect& rect : ship.rects){
			if (!rect.hitten){
				flag = false; // there still is some boys
				break;
			}
		}
		
		if (!flag) {
			break;
		}
	}
	
	if (flag) {
		setWinner('e');
		return true;
	}

	flag = true;

	// then check enems matrix
	for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            if (Enemy::getPrivateMatrix()[row][col] == 2) {
                return false; // Value 2 is present
            }
        }

		if (!flag) {
			break;
		}
    }

	if (flag) {
		setWinner('y');
		return true;
	}

	return false;
}

bool Game::isShipPlacementFinished(){
	for (const Ship& ship : ShipHandler::getShips()){
		if (ship.avaible){
			return false;
		}
	}

	return true;
}

void Game::startWindow()
{
	Renderer::allocBuffer();

	const wchar_t *className = L"Warship_window";

	WNDCLASS windowClass = {};

	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowCallback;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = className;

	if (!RegisterClass(&windowClass))
	{
		std::cerr << "Cannot register window" << std::endl;
		return;
	}

	DWORD windowStyle = WS_SYSMENU | WS_CAPTION;

	RECT windowRect = {};
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = windowWidth;
	windowRect.bottom = windowHeight;

	AdjustWindowRect(&windowRect, windowStyle, false);

	windowHandle = CreateWindowEx(
		0,
		className,
		(host ? (windowTitle + L"-HOST") : windowTitle).c_str(),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX) | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		0,
		0,
		hInstance,
		0);

	if (windowHandle)
	{
		running = true;

		Renderer::setWindowHandle(windowHandle);

		// init the clock
		LARGE_INTEGER cpu_frequency;
		QueryPerformanceFrequency(&cpu_frequency);

		LARGE_INTEGER last_counter;
		QueryPerformanceCounter(&last_counter);

		while (running)
		{
			LARGE_INTEGER current_counter;
			QueryPerformanceCounter(&current_counter);

			int64_t counter_elapsed = current_counter.QuadPart - last_counter.QuadPart;

			float delta = (float)counter_elapsed / (float)cpu_frequency.QuadPart; // in seconds

			last_counter = current_counter;

			// process windows messages
			MSG message;
			while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
			{
				if (message.message == WM_QUIT)
					running = false;

				TranslateMessage(&message);
				DispatchMessage(&message); // Send message to the WindowProc (WindowCallback)
			}

			// update & render

			Renderer::clear();

			getInstance().update(delta);

			HDC deviceContext = GetDC(windowHandle);

			Renderer::copyBufferToWindow(deviceContext, playerStep);

			ReleaseDC(windowHandle, deviceContext);
		}
	}
}