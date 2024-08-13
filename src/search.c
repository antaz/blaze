#include "search.h"
#include "board.h"
#include "eval.h"
#include "gen.h"
#include "move.h"
#include <inttypes.h>
#include <search.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static uint64_t start;    // time the search started
static uint64_t nodes;    // # of nodes searched so far
static uint64_t movetime; // how long should we search

static uint64_t time_ms()
{
    // current time in miliseconds
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1e3 + ts.tv_nsec / 1e6;
}

static uint64_t nps()
{
    // count nodes per second
    uint64_t elapsed = time_ms() - start;

    if (elapsed == 0)
        return 0;

    return (nodes * 1000) / elapsed;
}

static int stop(int depth)
{
    // check if we should stop searching
    if (movetime) {
        if (((time_ms() - start) * 2) >= movetime)
            return 1;
    } else {
        // check if we reached specified depth
        if (tc_data.depth && (depth >= tc_data.depth))
            return 1;
        // check if we reached specified nodes count
        if (tc_data.nodes && (nodes >= tc_data.nodes))
            return 1;
    }
    return 0;
}

static int search(struct board_t *board, int depth, uint16_t *pv)
{
    int val = 0;
    int best = -999999;
    uint16_t cpv[MAX_DEPTH] = {0};

    nodes++;

    if ((nodes & 1023) == 0)
        stop(depth);

    uint16_t moves[256];
    int count = gen_legal(board, moves);

    if (depth == 0) {
        return eval(board);
    }

    for (int i = 0; i < count; i++) {
        make(board, moves[i]);
        val = -search(board, depth - 1, cpv);
        take(board, moves[i]);

        if (val > best) {
            best = val;
            pv[0] = moves[i];
            memcpy(pv + 1, cpv, (MAX_DEPTH - depth) * sizeof(uint16_t));
        }
    }
    return val;
}

void go(struct board_t *board)
{
    int val = 0;
    uint16_t pv[MAX_DEPTH] = {0};

    // initialize time management
    start = time_ms();
    if (!tc_data.movetime) {
        movetime = tc_data.time[board->stm] / tc_data.movestogo +
                   tc_data.inc[board->stm];
    } else {
        movetime = tc_data.movetime;
    }

    // initialize nodes count
    nodes = 0;

    // iterative deepening
    for (int depth = 1; depth <= MAX_DEPTH; depth++) {

        val = search(board, depth, pv);

        // info line
        // TODO: add PV
        printf("info depth %d seldepth %d score cp %d nodes %" PRIu64
               " nps %" PRIu64 " time "
               "%" PRIu64 "\n",
               depth, depth, val, nodes, nps(), time_ms() - start);

        if (stop(depth)) {
            break;
        }
    }

    printf("bestmove %s\n", str_move(pv[0], board->stm));
}
