#include "board.h"

Board::Board(int length)
{
    this->_length = length;
}

bool Board::draw(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xbb, 0xbb, 0xbb, 0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xbb);

    int gridSideLength = this->_length / 16;
    for(int x = 1; x <= 15; x += 1) 
    {
        SDL_RenderDrawLine(renderer, x * gridSideLength, gridSideLength, x * gridSideLength, gridSideLength * 15);
        SDL_RenderDrawLine(renderer, gridSideLength, x * gridSideLength, gridSideLength * 15, x * gridSideLength);
    }

    int pointLength = this->_length / 64;
    static SDL_Rect points[5] = {
        {4 * gridSideLength - pointLength / 2, 4 * gridSideLength - pointLength / 2, pointLength, pointLength},
        {4 * gridSideLength - pointLength / 2, 12 * gridSideLength - pointLength / 2, pointLength, pointLength},
        {12 * gridSideLength - pointLength / 2, 4 * gridSideLength - pointLength / 2, pointLength, pointLength},
        {12 * gridSideLength - pointLength / 2, 12 * gridSideLength - pointLength / 2, pointLength, pointLength},
        {8 * gridSideLength - pointLength / 2, 8 * gridSideLength - pointLength / 2, pointLength, pointLength},
    };
    SDL_RenderFillRects(renderer, points, 5);

    return true;
}
