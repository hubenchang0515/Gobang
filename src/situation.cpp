#include "situation.h"
#include <cstring>

Situation::Situation(int length) :
    _piece(length),
    _ai(&(this->_map))
{
    memset(this->_map, 0, sizeof(Piece::Color[15][15]));
    this->_turn = Piece::Color::BLACK;
    this->_length = length;
    this->_pointer.x = -1;
    this->_pointer.y = -1;
    this->winner = Piece::Color::NONE;
}

bool Situation::draw(SDL_Renderer* renderer)
{
    for(int row = 0; row < 15; row++)
    {
        for(int col = 0; col < 15; col++)
        {
            this->_piece.draw(renderer, row, col, this->_map[row][col]);
        }
    }
    this->_piece.draw(renderer, this->_pointer.y, this->_pointer.x, Piece::Color::BLACK);

    if(this->winner != Piece::Color::NONE)
    {
        if(this->winner == Piece::Color::BLACK)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You win", nullptr);
        }
        else if(this->winner == Piece::Color::WHITE)
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Game Over", "You loss", nullptr);
        }
        memset(this->_map, 0, sizeof(Piece::Color[15][15]));
        this->_turn = Piece::Color::BLACK;
        this->_pointer.x = -1;
        this->_pointer.y = -1;
        this->winner = Piece::Color::NONE;
    }
    return true;
}

bool Situation::checkWin(Piece::Color turn)
{
    if(this->checkRow(turn) || this->checkCol(turn) || this->checkBackSlash(turn) || this->checkSlash(turn))
    {
        return true;
    }
    return false;
}

bool Situation::update(SDL_Event* event)
{
    if(this->_turn == Piece::Color::BLACK)
    {
        if(this->player(event))
        {
            if(this->checkWin(Piece::Color::BLACK))
            {
                this->winner = Piece::Color::BLACK;
            }
            this->_turn = Piece::Color::WHITE;
        }
    }
    else
    {
        this->_pointer.x = -1;
        this->_pointer.y = -1;
        if(this->_ai.update())
        {
            if(this->checkWin(Piece::Color::WHITE))
            {
                this->winner = Piece::Color::WHITE;
            }
            this->_turn = Piece::Color::BLACK;
        }
    }
    
    return true;
}

bool Situation::player(SDL_Event* event)
{
    int gridSideLength = this->_length / 16;
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) 
    {
        int row = (event->button.y - gridSideLength/2) / gridSideLength;
		int col = (event->button.x - gridSideLength/2) / gridSideLength;
		
		if(row >= 0 && row <= 14 && col >= 0 && col <= 14 && this->_map[row][col] == Piece::Color::NONE)
		{	
			this->_map[row][col] = Piece::Color::BLACK;
            return true;
		}
    }
    if(event->type == SDL_MOUSEMOTION)
    {
        int row = (event->button.y - gridSideLength/2) / gridSideLength;
		int col = (event->button.x - gridSideLength/2) / gridSideLength;
		
		if(row >= 0 && row <= 14 && col >= 0 && col <= 14 && this->_map[row][col] == Piece::Color::NONE)
		{	
			this->_pointer.x = col;
            this->_pointer.y = row;
		}
    }

    return false;
}

bool Situation::checkRow(Piece::Color turn)
{
    for(int row = 0; row < 15; row++) 
    {
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
                    return true;
                }
                length = 0;
            }
        }

        // 末端
        if(length > 5)
        {
            return true;
        }
    }
    
    return false;
}


bool Situation::checkCol(Piece::Color turn)
{
    for(int col = 0; col < 15; col++) 
    {
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
                    return true;
                }
                length = 0;
            }

            // 末端
            if(length >= 5)
            {
                return true;
            }
        }
    }
    
    return false;
}


bool Situation::checkBackSlash(Piece::Color turn)
{
    int score = 0;

    // 右上角
    for(int col = 0; col < 15; col++)
    {
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
                    return true;
                }
                length = 0;
            }
        }

        // 末端
        if(length >= 5)
        {
            return true;
        }
    }

    // 左下角
    for(int row = 1; row < 15; row++)
    {
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
                    return true;
                }
                length = 0;
            }
            
        }

        // 末端
        if(length >= 5)
        {
            return true;
        }
    }

    return score;
}


bool Situation::checkSlash(Piece::Color turn)
{
    int score = 0;

    // 左上角
    for(int col = 0; col < 15; col++)
    {
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
                    return true;
                }
            }
        }

        // 末端
        if(length >= 5)
        {
            return true;
        }
    }

    // 右下角
    for(int row = 1; row < 15; row++)
    {
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
                    return true;
                }
                length = 0;
            }
        }

        // 末端
        if(length >= 5)
        {
            return true;
        }
    }

    return score;
}