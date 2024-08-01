#include "search.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "eval.h"
#include <search.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

uint16_t PV[MAX_DEPTH];

uint64_t time_ms()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1e3 + ts.tv_nsec / 1e6;
}

void deepen(struct board_t *board, struct clck_t *clock)
{
    // iterative deepening
    // int val = 0;

    // init move and search time
    uint64_t start = time_ms();
    int movetime = 0;
    if (clock->movetime != 0) {
        movetime = clock->movetime;
    } else {
        movetime = clock->time[board->turn] / clock->movestogo +
                   clock->inc[board->turn];
    }

    for (int depth = 2; depth <= MAX_DEPTH; depth++) {

        search(board, depth, PV);
        // if time is over return bestmove and break;
        if ((int)(time_ms() - start) > movetime) {
            break;
        }
    }

    printf("bestmove %s\n", str_move(PV[0], board->turn));
}

int search(struct board_t *board, int depth, uint16_t *pv) {
    int val = 0;
    int best = -10000;
    uint16_t cpv[MAX_DEPTH] = {0};

    uint16_t moves[256];
    int count = gen_legal(board, moves);

    if (depth == 0) {
        return eval(board);
    }

    for (int i = 0; i < count; i++) {
        make(board, moves[i]);
        val = -search(board, depth - 1, cpv);
        unmake(board, moves[i]);

        if (val > best) {
            best = val;
            pv[0] = moves[i];
            memcpy(pv + 1, cpv, (MAX_DEPTH - depth) * sizeof(uint16_t));
        }
    }
    return val;
}
