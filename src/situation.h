#ifndef GOBANG_SITUATION_H
#define GOBANG_SITUATION_H

#include "game.h"
#include "piece.h"
#include "ai.h"

class Situation : public AbstractObject
{
public:
    Situation(int length);
    virtual bool draw(SDL_Renderer* renderer);
    virtual bool update(SDL_Event* event);
    bool player(SDL_Event* event);
    Piece::Color _map[15][15] = {{Piece::Color::NONE}};

private:
    int _length;
    SDL_Point _pointer;
    Piece::Color _turn;
    Piece _piece;
    Ai _ai;
};

#endif
