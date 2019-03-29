#include "game.h"
#include "board.h"
#include "situation.h"

#define GRID_SIDE_LENGTH 40
#define SIDE_LENGTH 40 * 16


int main(int argc, char* argv[])
{
    Game game(SIDE_LENGTH, SIDE_LENGTH, "Gobang");
    Board board(SIDE_LENGTH);
    Situation situation(SIDE_LENGTH);
    game.addObject(&board);
    game.addObject(&situation);
    game.execute(argc, argv);

    return 0;
}