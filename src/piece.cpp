#include "piece.h"
#include <cassert>
#include <cmath>

Piece::Piece(int length)
{
    this->_length = length;
}


void Piece::draw(SDL_Renderer* renderer, int row, int col, Color color)
{
    if(row < 0 || row > 14 || col < 0 || col > 14)
        return;

    if(color != Piece::Color::WHITE && color != Piece::Color::BLACK)
        return;

    if(color == Piece::Color::BLACK)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    else if(color == Piece::Color::WHITE)
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    
    int gridSideLength = this->_length / 16;
    int rx = gridSideLength * (col + 1);
    int ry = gridSideLength * (row + 1);
    int r = gridSideLength - 2;
    for(int i = -r; i <= r - 2; i++)
    {
        int delta = pow(18*18 - pow(i , 2) , 0.5);
        SDL_RenderDrawLine(renderer, rx-delta, ry + i, rx+delta, ry + i);
    }
}