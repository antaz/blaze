#include "search.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

uint64_t time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1e3 + ts.tv_nsec / 1e6;
}

void search(struct board_t *board, struct clck_t *clock)
{

    // init move and search time
    uint64_t start = time_ms();
    int movetime = 0;
    if (clock->movetime != 0) {
        movetime = clock->movetime;
    } else {
        movetime = clock->time[board->turn] / clock->movestogo +
                   clock->inc[board->turn];
    }

    // generate legal moves
    uint16_t moves[256];
    gen_legal(board, moves);

    for (int depth = 2; depth <= MAX_DEPTH; depth++) {

        // if time is over return bestmove and break;
        if ((int)(time_ms() - start) > movetime) {
            break;
        }
    }

    printf("bestmove %s\n", str_move(moves[0], board->turn));
}
