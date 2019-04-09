#include "ai.h"
#include <algorithm>
#include <cstdint>

// 与 const int Ai::scoreList 对应，用来查表
enum class ScoreIndex
{
    ZERO = 0,
    CLOSE_ONE,       // CLOSE_表示两侧均被封堵
    BLOCK_ONE,       // BLOCK_表示单侧被封堵
    ONE,
    CLOSE_TWO,
    BLOCK_TWO,
    TWO,
    CLOSE_THREE,
    BLOCK_THREE,
    THREE,
    CLOSE_FOUR,
    BLOCK_FOUR,
    FOUR,
    FIVE,
};

// 与 enum class ScoreIndex 对应
const int Ai::scoreList[] = 
{
    0,          // ZERO
    0,          // CLOSE_ONE
    1,          // BLOCK_ONE
    5,          // ONE
    0,          // CLOSE_TWO
    5,          // ...
    10,
    0,
    50,
    500,
    0,
    500,
    1000,
    10000
};


int Ai::getScoreIndex(int length, Piece::Color turn, Piece::Color prev, Piece::Color next)
{
    int index = 0;
    switch(length)
    {
        case 0 :
            return static_cast<int>(ScoreIndex::ZERO);

        case 1 :
            index = static_cast<int>(ScoreIndex::ONE);
            break;

        case 2 :
            index = static_cast<int>(ScoreIndex::TWO);;
            break;

        case 3 :
            index = static_cast<int>(ScoreIndex::THREE);
            break;

        case 4 :
            index = static_cast<int>(ScoreIndex::FOUR);
            break;

        default: // >= 5
            return static_cast<int>(ScoreIndex::FIVE);
    }

    if(prev != Piece::Color::NONE)
    {
        index -= 1;
    }
    
    if(next != Piece::Color::NONE)
    {
        index -= 1;
    }

    return index;
}

Ai::Ai(Piece::Color (*map)[15][15])
{
    this->_map = map;
}

bool Ai::update()
{
    int score = INT_MIN;
    int row_result = 0;
    int col_result = 0;
    for(int row = 0; row < 15; row++)
    {
        for(int col = 0; col < 15; col++)
        {
            if((*(this->_map))[row][col] == Piece::Color::NONE)
            {
                (*(this->_map))[row][col] = Piece::Color::WHITE;

                int score_step = this->score(Piece::Color::WHITE) - this->score(Piece::Color::BLACK);
                if(score_step > score)
                {
                    score = score_step;
                    row_result = row;
                    col_result = col;
                }

                (*(this->_map))[row][col] = Piece::Color::NONE;
            }
        }
    }

    (*(this->_map))[row_result][col_result] = Piece::Color::WHITE;
    
    return true;
}

int Ai::score(Piece::Color turn)
{
    int score = 0;
    score += this->rowScore(turn);
    score += this->colScore(turn);
    score += this->slashScore(turn);
    score += this->backSlashScore(turn);
    return score;
}

int Ai::rowScore(Piece::Color turn)
{
    int score = 0;
    for(int row = 0; row < 15; row++) 
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int col = 0; col < 15; col++)
        {
            if((*(this->_map))[row][col] == turn) 
            {
                length += 1;
            }
            else
            {
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, (*(this->_map))[row][col]) ];
                length = 0;
                prev = (*(this->_map))[row][col];
            }
        }

        // 末端
        score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn)) ];
    }
    
    return score;
}


int Ai::colScore(Piece::Color turn)
{
    int score = 0;
    for(int col = 0; col < 15; col++) 
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int row = 0; row < 15; row++)
        {
            if((*(this->_map))[row][col] == turn) 
            {
                length += 1;
            }
            else
            {
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, (*(this->_map))[row][col]) ];
                length = 0;
                prev = (*(this->_map))[row][col];
            }
        }

        // 末端
        score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn)) ];
    }
    
    return score;
}


int Ai::slashScore(Piece::Color turn)
{
    int score = 0;

    // 右上角
    for(int col = 0; col < 15; col++)
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int n = 0; n < 15 - col; n++)
        {
            if((*(this->_map))[n][col+n] == turn) 
            {
                length += 1;
            }
            else
            {
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, (*(this->_map))[n][col+n]) ];
                length = 0;
                prev = (*(this->_map))[n][col+n];
            }
        }

        // 末端
        score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn)) ];
    }

    // 左下角
    for(int row = 1; row < 15; row++)
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int n = 0; n < 15 - row; n++)
        {
            if((*(this->_map))[row+n][n] == turn)
            {
                length += 1;
            }
            else
            {
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, (*(this->_map))[row+n][n]) ];
                length = 0;
                prev = (*(this->_map))[row+n][n];
            }
            
        }

        // 末端
        score += Ai::scoreList[Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn))];
    }

    return score;
}


int Ai::backSlashScore(Piece::Color turn)
{
    int score = 0;

    // 左上角
    for(int col = 0; col < 15; col++)
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int n = 0; n < 15 - col; n++)
        {
            if((*(this->_map))[n][col+n] == turn)
            {
                length += 1;
            }
            else
            {
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, (*(this->_map))[n][col+n]) ];
                length = 0;
                prev = (*(this->_map))[n][col+n];
            }
        }

        // 末端
        score += Ai::scoreList[Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn))];
    }

    // 右下角
    for(int row = 1; row < 15; row++)
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int n = 0; n < 15 - row; n++)
        {
            if((*(this->_map))[row+n][n] == turn)
            {
                length += 1;
            }
            else
            {
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, (*(this->_map))[row+n][n]) ];
                length = 0;
                prev = (*(this->_map))[row+n][n];
            }
        }

        // 末端
        score += Ai::scoreList[Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn))];
    }

    return score;
}