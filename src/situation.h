#ifndef GOBANG_SITUATION_H
#define GOBANG_SITUATION_H

#include "game.h"
#include "piece.h"
#include "ai.h"

class Situation : public AbstractObject
{
public:
    Situation(int length);
    ~Situation();
    virtual bool draw(SDL_Renderer* renderer);
    virtual bool update(SDL_Event* event);
    bool player(SDL_Event* event);
    bool checkWin(Piece::Color turn);
    bool checkRow(Piece::Color turn);
    bool checkCol(Piece::Color turn);
    bool checkSlash(Piece::Color turn);
    bool checkBackSlash(Piece::Color turn);
    Piece::Color _map[15][15] = {{Piece::Color::NONE}};

private:
    int _length;
    SDL_Point _pointer;
    SDL_Point _playerLast;
    SDL_Point _aiLast;
    Piece::Color _turn;
    Piece _piece;
    Ai _ai;
    Piece::Color winner;
    bool aiRunning;
    SDL_mutex* aiRunningMutex;

    static int updateThread(void* data);
};

#endif
