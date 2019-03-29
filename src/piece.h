#ifndef GOBANG_PIECE_H
#define GOBANG_PIECE_H

#include <SDL2/SDL.h>

class Piece
{
public:
    enum class Color
    {
        NONE = 0,
        WHITE,
        BLACK
    };

    Piece(int length);
    void draw(SDL_Renderer* renderer, int row, int col, Color color);

    

private:
    int _length;
};

#endif