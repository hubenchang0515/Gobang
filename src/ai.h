#ifndef GOBANG_AI_H
#define GOBANG_AI_H

#include "piece.h"

class Ai
{
public:
    Ai(Piece::Color (*map)[15][15]);
    bool update(SDL_Point* point);

    /* 极大值搜索 */
    int max(Piece::Color turn, int step, int alpha, int* row=nullptr, int* col=nullptr);

    /* 极小值搜索 */
    int min(Piece::Color turn, int step, int beta, int* row=nullptr, int* col=nullptr);

    /* 评分函数 */
    int score(Piece::Color turn);

    /* 计算各个行的总分 */
    int rowScore(Piece::Color turn);
    
    /* 计算各个列的总分 */
    int colScore(Piece::Color turn);

    /* 计算各个正斜线(/)的总分 */
    int slashScore(Piece::Color turn);

    /* 计算各个反斜线(\)的总分 */
    int backSlashScore(Piece::Color turn);

private:
    Piece::Color (*_situationMap)[15][15];
    Piece::Color _map[15][15];
    static int indexList[15*15][2];

    static const int scoreList[];
    static int getScoreIndex(int length, Piece::Color turn, Piece::Color prev, Piece::Color next);
};

#endif