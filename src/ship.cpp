#include "../headers/ship.h"
#include "../headers/player.h"

bool ShipHandler::IsMouseInRect(int mouseX, int mouseY, const Rect& rect) {
    return (mouseX >= rect.x && mouseX <= (rect.x + rect.width) &&
            mouseY >= rect.y && mouseY <= (rect.y + rect.height));
}

Ship* ShipHandler::findShipByMouse(int mouseX, int mouseY) {
    auto& ships = getShips();
    for (auto& ship : ships) {
        for (const Rect& rect : ship.rects) {
            if (IsMouseInRect(mouseX, mouseY, rect)) {
                std::cout << "Ship: ship found and it is: " << ship.id << std::endl;
                return &ship; // Mouse is within a rectangle of this ship
            }
        }
    }
    return nullptr; // Mouse is not within any ship
}

int ShipHandler::getShipLengthById(const Ship& ship){
    return ship.rects.size() * Conf::size;
}

void ShipHandler::fillUpShips(){
    int start_pos_x = 50;
    int start_pos_y = 550;
    std::vector<Ship>& ships = ShipHandler::getShips();

    // big_ship
    ships.push_back(
        { "big_ship",
            { 
                {start_pos_x, start_pos_y, Conf::size, Conf::size},
                {start_pos_x + Conf::size, start_pos_y, Conf::size, Conf::size},
                {start_pos_x + 2 * Conf::size, start_pos_y, Conf::size, Conf::size},
                {start_pos_x + 3 * Conf::size, start_pos_y, Conf::size, Conf::size},
                {start_pos_x + 4 * Conf::size, start_pos_y, Conf::size, Conf::size}
            }
        }
    );

    start_pos_x += 5 * Conf::size + Conf::size;

    // medium_ship
    for (int i = 0; i < 2; i++) {
        ships.push_back(
            { 
                "medium_ship_" + std::to_string(i + 1),
                { 
                    {start_pos_x, start_pos_y, Conf::size, Conf::size},
                    {start_pos_x + Conf::size, start_pos_y, Conf::size, Conf::size},
                    {start_pos_x + 2 * Conf::size, start_pos_y, Conf::size, Conf::size},
                    {start_pos_x + 3 * Conf::size, start_pos_y, Conf::size, Conf::size}
                }
            }
        );
    };
    start_pos_x = 50;
    start_pos_y = 650;

    // small_ship
    for (int i = 0; i < 3; i++) {
        ships.push_back(
            { 
                "small_ship_" + std::to_string(i + 1),
                { 
                    {start_pos_x, start_pos_y, Conf::size, Conf::size},
                    {start_pos_x + Conf::size, start_pos_y, Conf::size, Conf::size}
                }
            }
        );
    };

    start_pos_x += Conf::size * 3;
    // tiny_ship
    for (int i = 0; i < 5; i++) {
        ships.push_back(
            { 
                "tiny_ship_" + std::to_string(i + 1),
                { 
                    {start_pos_x, start_pos_y, Conf::size, Conf::size}
                }
            }
        );
    };
};

bool ShipHandler::canPlaceShip(int startX, int startY, char orientation, int shipLength) {
    if (orientation == 'h') {
		std::cout << "Orientation: X" << std::endl;
        if ((startX + shipLength) > 10) {
			std::cout << " Ship will hit " << std::endl;
            return false;
        }

        for (int x = startX; x < startX + shipLength; x++) { // Check for collisions with other ships horizontally
            if (Player::getPrivateMatrix()[startY][x] == 2) {
				std::cout << " Ship collides with another ship " << std::endl;
                return false; // Ship collides with another ship
            }
        }

		// Check for minimal distance between ships horizontally
        for (int x = startX - 1; x < startX + shipLength + 1; ++x) {
            for (int y = startY - 1; y < startY + 1 + 1; ++y) {
                if (x >= 0 && x < 10 && y >= 0 && y < 10 && Player::getPrivateMatrix()[y][x] != 0) {
                    return false; // Too close to another ship
                }
            }
        }
    } else if (orientation == 'v') {
        if (startY + shipLength > 10) { // Check if the ship can fit vertically
            return false; // Ship goes beyond the board's bottom edge
        }
        for (int y = startY; y < startY + shipLength; y++) {  // Check for collisions with other ships vertically
            if (Player::getPrivateMatrix()[y][startX] == 2) {
                return false; // Ship collides with another ship
            }
        }

		// Check for minimal distance between ships vertically
        for (int y = startY - 1; y < startY + shipLength + 1; ++y) {
            for (int x = startX - 1; x < startX + 1 + 1; ++x) {
                if (x >= 0 && x < 10 && y >= 0 && y < 10 && Player::getPrivateMatrix()[y][x] != 0) {
                    return false; // Too close to another ship
                }
            }
        }
    } else {
        return false;
    }
    return true; // Ship can be placed
}

bool ShipHandler::isDestroyed(Ship* ship) {// last two params is x and y where matrix starts
    for (const Rect& rect : ship->rects) {
        if (Player::getPrivateMatrix()[rect.placeOnMatrix.y][rect.placeOnMatrix.x] != 1) {
            return false;
        }
    }
    return true;
}

Ship* ShipHandler::getShipFromMatrix(int x, int y) {
    std::vector<Ship>& ships = getShips(); // ships are placed only on player matrix as objects, on bot there not tship objecst

    for (Ship& ship : ships) {
        for (Rect& rect: ship.rects) {
            if (rect.placeOnMatrix.x == x && rect.placeOnMatrix.y == y) {
                std::cout << ship.id << std::endl;
                return &ship;
            }
        }
    }

    return nullptr;
}

std::vector<ShipType> ShipHandler::getShipTypes(){
    std::vector<ShipType> shipTypes = {
        {5, 1}, // One 5-rectangle ship
        {4, 2}, // Two 4-rectangle ships
        {2, 3}, // Three 2-rectangle ships
        {1, 5} // Five 1-rectangle ships
    };
    return shipTypes;
}