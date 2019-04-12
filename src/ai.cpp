#include "ai.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cstdint>
#include <climits>
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
    5000,
    50000
};

// 从中间往外围遍历
int Ai::indexList[15*15][2] = {0};


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

Ai::Ai(Piece::Color (*map)[15][15], Piece::Color turn)
{
    this->_sourceMap = map;
    int i = 0;
    Ai::indexList[0][0] = 7;
    Ai::indexList[0][1] = 7;
    for(int r = 1; r < 8; r++)
    {
        for(int col = 7-r; col < 7+r+1; col++)
        {
            Ai::indexList[i][0] = 7 - r; // row
            Ai::indexList[i][1] = col;
            i++;
            Ai::indexList[i][0] = 7 + r; // row
            Ai::indexList[i][1] = col;
            i++;
        }

        for(int row = 7-r+1; row < 7+r; row++)
        {
            Ai::indexList[i][0] = row;
            Ai::indexList[i][1] = 7 - r; // col
            i++;
            Ai::indexList[i][0] = row; 
            Ai::indexList[i][1] = 7 + r; // col
            i++;
        }
    }
    SDL_Log("i = %d, %d", i, 15*15);
}

int totalSteps = 0;
bool Ai::update(SDL_Point* point)
{
    int row = -1;
    int col = -1;
    totalSteps = 0;
    memcpy(&(this->_map), this->_sourceMap, sizeof(Piece::Color[15][15]));
    int score = this->max(Piece::Color::WHITE, 4, INT_MAX, &row, &col);
    SDL_Log("use %d steps", totalSteps);
    this->_map[row][col] = Piece::Color::WHITE;
    (*(this->_sourceMap))[row][col] = Piece::Color::WHITE;
    SDL_Log("White go (%d,%d)", row, col);
    SDL_Log("Score : %d", score);
    SDL_Log("Black Score : %d", this->score(Piece::Color::BLACK));
    SDL_Log("White Score : %d", this->score(Piece::Color::WHITE));
    point->x = col;
    point->y = row;
    
    return true;
}

int Ai::max(Piece::Color turn, int step, int alpha, int* row, int* col)
{
    int score = INT_MIN;
    int row_result = 0;
    int col_result = 0;
    for(int i = 0; i < 15*15; i++)
    {
        int row = Ai::indexList[i][0];
        int col = Ai::indexList[i][1];

        // foolish加速，如果该空位周围8个位置 均无子，则跳过改点计算
        if(row > 1 && row < 14 && col > 1 && col < 14 &&
            this->_map[row-1][col-1] == Piece::Color::NONE &&
            this->_map[row-1][col] == Piece::Color::NONE &&
            this->_map[row-1][col+1] == Piece::Color::NONE &&
            this->_map[row][col-1] == Piece::Color::NONE &&
            this->_map[row][col+1] == Piece::Color::NONE &&
            this->_map[row+1][col-1] == Piece::Color::NONE &&
            this->_map[row+1][col] == Piece::Color::NONE &&
            this->_map[row+1][col+1] == Piece::Color::NONE)
        {
            continue;
        }

        if(this->_map[row][col] == Piece::Color::NONE)
        {
            this->_map[row][col] = turn;

            int score_step = INT_MIN;
            if(step <= 1)
            {
                score_step = this->score(turn) - this->score(COLOR_ANOTHER(turn));
                totalSteps += 1;
            }
            else if(this->score(turn) >= Ai::scoreList[ static_cast<int>(ScoreIndex::FIVE) ])
            {
                score_step = Ai::scoreList[ static_cast<int>(ScoreIndex::FIVE) ] + 100 * step;
                if(score_step > score)
                {
                    score = score_step;
                    row_result = row;
                    col_result = col;
                }

                this->_map[row][col] = Piece::Color::NONE;
                break;
            }
            else
            {
                score_step = this->min(turn, step-1, score);
            }

            // alpha剪枝
            if(score_step >= alpha)
            {
                this->_map[row][col] = Piece::Color::NONE;
                
                return alpha;
            }

            if(score_step > score)
            {
                score = score_step;
                row_result = row;
                col_result = col;
            }

            this->_map[row][col] = Piece::Color::NONE;
        }
    }

    if(row != nullptr)
    {
        *row = row_result;
    }

    if(col != nullptr)
    {
        *col = col_result;
    }

    return score;
}

int Ai::min(Piece::Color turn, int step, int beta, int* row, int* col)
{
    int score = INT_MAX;
    int row_result = 0;
    int col_result = 0;
    for(int i = 0; i < 15*15; i++)
    {
        int row = Ai::indexList[i][0];
        int col = Ai::indexList[i][1];

        // foolish加速，如果该空位周围8个位置 均无子，则跳过改点计算
        if( row > 1 && row < 14 && col > 1 && col < 14 &&
            this->_map[row-1][col-1] == Piece::Color::NONE &&
            this->_map[row-1][col] == Piece::Color::NONE &&
            this->_map[row-1][col+1] == Piece::Color::NONE &&
            this->_map[row][col-1] == Piece::Color::NONE &&
            this->_map[row][col+1] == Piece::Color::NONE &&
            this->_map[row+1][col-1] == Piece::Color::NONE &&
            this->_map[row+1][col] == Piece::Color::NONE &&
            this->_map[row+1][col+1] == Piece::Color::NONE)
        {
            continue;
        }

        if(this->_map[row][col] == Piece::Color::NONE)
        {
            this->_map[row][col] = COLOR_ANOTHER(turn);

            int score_step = INT_MAX;
            if(step <= 1)
            {
                score_step = this->score(turn) - this->score(COLOR_ANOTHER(turn));
                totalSteps += 1;
            }
            else if(this->score(COLOR_ANOTHER(turn)) >= Ai::scoreList[ static_cast<int>(ScoreIndex::FIVE) ])
            {
                score_step = - Ai::scoreList[ static_cast<int>(ScoreIndex::FIVE) ] - 100 * step;
                if(score_step < score)
                {
                    score = score_step;
                    row_result = row;
                    col_result = col;
                }

                this->_map[row][col] = Piece::Color::NONE;
                break;
            }
            else
            {
                score_step = this->max(turn, step-1, score);
            }

            // beta剪枝
            if(score_step <= beta)
            {
                this->_map[row][col] = Piece::Color::NONE;
                
                return beta;
            }

            if(score_step < score)
            {
                score = score_step;
                row_result = row;
                col_result = col;
            }

            this->_map[row][col] = Piece::Color::NONE;
        }
    }

    if(row != nullptr)
    {
        *row = row_result;
    }

    if(col != nullptr)
    {
        *col = col_result;
    }

    return score;
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
            if(this->_map[row][col] == turn) 
            {
                length += 1;
            }
            else
            {
                if(length >= 5)
                {
                    return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
                }
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, this->_map[row][col]) ];
                length = 0;
                prev = this->_map[row][col];
            }
        }

        // 末端
        if(length >= 5)
        {
            return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
        }
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
            if(this->_map[row][col] == turn) 
            {
                length += 1;
            }
            else
            {
                if(length >= 5)
                {
                    return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
                }
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, this->_map[row][col]) ];
                length = 0;
                prev = this->_map[row][col];
            }
        }

        // 末端
        if(length >= 5)
        {
            return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
        }
        score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn)) ];
    }
    
    return score;
}


int Ai::backSlashScore(Piece::Color turn)
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
            if(this->_map[n][col+n] == turn) 
            {
                length += 1;
            }
            else
            {
                if(length >= 5)
                {
                    return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
                }
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, this->_map[n][col+n]) ];
                length = 0;
                prev = this->_map[n][col+n];
            }
        }

        // 末端
        if(length >= 5)
        {
            return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
        }
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
            if(this->_map[row+n][n] == turn)
            {
                length += 1;
            }
            else
            {
                if(length >= 5)
                {
                    return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
                }
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, this->_map[row+n][n]) ];
                length = 0;
                prev = this->_map[row+n][n];
            }
            
        }

        // 末端
        if(length >= 5)
        {
            return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
        }
        score += Ai::scoreList[Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn))];
    }

    return score;
}


int Ai::slashScore(Piece::Color turn)
{
    int score = 0;

    // 左上角
    for(int col = 0; col < 15; col++)
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int n = 0; n < col+1; n++)
        {
            if(this->_map[n][col-n] == turn)
            {
                length += 1;
            }
            else
            {
                if(length >= 5)
                {
                    return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
                }
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, this->_map[n][col-n]) ];
                length = 0;
                prev = this->_map[n][col-n];
            }
        }

        // 末端
        if(length >= 5)
        {
            return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
        }
        score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn)) ];
    }

    // 右下角
    for(int row = 1; row < 15; row++)
    {
        // 由于最开始在边缘上，前面不能落子，相当于有对手的子
        Piece::Color prev = COLOR_ANOTHER(turn); 
        int length = 0;

        for(int n = 0; n < 15 - row; n++)
        {
            if(this->_map[row+n][14-n] == turn)
            {
                length += 1;
            }
            else
            {
                if(length >= 5)
                {
                    return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
                }
                score += Ai::scoreList[ Ai::getScoreIndex(length, turn, prev, this->_map[row+n][15-row-n]) ];
                length = 0;
                prev = this->_map[row+n][15-row-n];
            }
        }

        // 末端
        if(length >= 5)
        {
            return Ai::scoreList[static_cast<int>(ScoreIndex::FIVE)];
        }
        score += Ai::scoreList[Ai::getScoreIndex(length, turn, prev, COLOR_ANOTHER(turn))];
    }

    return score;
}
