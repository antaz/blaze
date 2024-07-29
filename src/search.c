#include "board.h"
#include "gen.h"
#include "move.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void search(struct board_t *board) {
    uint16_t moves[256];
    int count = 0;

    count = gen_legal(board, moves);

    // initialize srand
    srand(time(NULL));

    int idx = rand() % count;
    char *bestmove = str_move(moves[idx], board->turn);

    // send the first move
    printf("bestmove %s\n", bestmove);
}
