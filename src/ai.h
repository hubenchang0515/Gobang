#ifndef GOBANG_AI_H
#define GOBANG_AI_H

#include "piece.h"

class Ai
{
public:
    Ai(Piece::Color (*map)[15][15]);
    bool update();

private:
    Piece::Color (*_map)[15][15];
};

#endif