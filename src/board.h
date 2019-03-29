#ifndef GOBANG_BOARD_H
#define GOBANG_BOARD_H

#include "game.h"

class Board : public AbstractObject
{
public:
    Board(int length);
    virtual bool draw(SDL_Renderer* renderer);

private:
    int _length;
};

#endif