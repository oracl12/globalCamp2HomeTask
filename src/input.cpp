
#include "../headers/input.h"

Input::Position &Input::getMousePosition(LPARAM lParam)
{
    static Position mousePosition;
    mousePosition.x = GET_X_LPARAM(lParam);
    mousePosition.y = GET_Y_LPARAM(lParam);

    return mousePosition;
};