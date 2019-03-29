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

    return true;
}

bool Situation::update(SDL_Event* event)
{
    if(this->_turn == Piece::Color::BLACK)
    {
        if(this->player(event))
        {
            this->_turn = Piece::Color::WHITE;
        }
    }
    else
    {
        this->_pointer.x = -1;
        this->_pointer.y = -1;
        if(this->_ai.update())
        {
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