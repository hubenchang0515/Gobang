#ifndef GOBANG_AI_H
#define GOBANG_AI_H

#include "piece.h"

class Ai
{
public:
    Ai(Piece::Color (*map)[15][15]);
    bool update();
    int score(Piece::Color turn);

    /* 计算各个行的总分 */
    int rowScore(Piece::Color turn);
    
    /* 计算各个列的总分 */
    int colScore(Piece::Color turn);

    /* 计算各个正斜线(\)的总分 */
    int slashScore(Piece::Color turn);

    /* 计算各个反斜线(/)的总分 */
    int backSlashScore(Piece::Color turn);

private:
    Piece::Color (*_map)[15][15];

    static const int scoreList[];
    static int getScoreIndex(int length, Piece::Color turn, Piece::Color prev, Piece::Color next);
};

#endif