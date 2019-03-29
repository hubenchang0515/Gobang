#include "ai.h"

Ai::Ai(Piece::Color (*map)[15][15])
{
    this->_map = map;
}

bool Ai::update()
{

    return true;
}