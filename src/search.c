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

static uint64_t start;      // time the search started
static uint64_t nodes;      // # of nodes searched so far
static uint64_t movetime;   // how long should we search
static int stop_search = 0; // search stop flag

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

static void stop(int depth)
{
    // check if we should stop searching
    if (movetime) {
        if ((time_ms() - start) >= movetime)
            stop_search = 1;
    } else {
        // check if we reached specified depth
        if (tc_data.depth && (depth >= tc_data.depth))
            stop_search = 1;
        // check if we reached specified nodes count
        if (tc_data.nodes && (nodes >= tc_data.nodes))
            stop_search = 1;
    }
}

static int rep(struct board_t *board)
{
    int r = 0;
    for (int i = board->hply - board->fifty; i < board->hply; i++) {
        if (board->hist[i].hash == board->hash) {
            r++;
        }
    }
    return r;
}

static void print_pv(struct pv_t pv, int stm)
{
    int s = stm;

    for (int i = 0; i < pv.count; i++) {
        printf("%s ", str_move(pv.moves[i], s));
        s ^= BLACK;
    }
}

static int search(struct board_t *board, int depth, struct pv_t *pv)
{
    int val = 0;
    int best = -2 * 9999;
    struct pv_t cpv;
    cpv.count = 0;

    if ((nodes & 1023) == 0)
        stop(depth);

    if (depth == 0) {
        pv->count = 0;
        return eval(board);
    }

    nodes++;

    if (board->ply && rep(board)) {
        pv->count = 0;
        return 0;
    }

    uint16_t moves[256];
    int count = gen_legal(board, moves);

    for (int i = 0; i < count; i++) {
        make(board, moves[i]);
        val = -search(board, depth - 1, &cpv);
        take(board, moves[i]);

        if (stop_search)
            break;

        if (val > best) {
            best = val;
            pv->moves[0] = moves[i];
            memcpy(pv->moves + 1, cpv.moves, cpv.count * sizeof(uint16_t));
            pv->count = cpv.count + 1;
        }
    }

    if (board->fifty >= 100) {
        pv->count = 0;
        return 0;
    }

    if (count == 0) {
        pv->count = 0;
        if (check(board)) {
            return -9999 + board->ply;
        } else {
            return 0;
        }
    }

    return best;
}

void go(struct board_t *board)
{
    int val = 0;
    int max_depth = MAX_DEPTH;
    stop_search = 0;
    struct pv_t pv;
    pv.count = 0;

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

    // reset ply
    board->ply = 0;

    if (tc_data.depth)
        max_depth = tc_data.depth;

    // iterative deepening
    for (int depth = 1; depth <= max_depth; depth++) {

        val = search(board, depth, &pv);

        // info line
        printf("info depth %d seldepth %d score cp %d nodes %" PRIu64
               " nps %" PRIu64 " time "
               "%" PRIu64 " pv ",
               depth, depth, val, nodes, nps(), time_ms() - start);

        print_pv(pv, board->stm);
        printf("\n");

        if (stop_search) {
            break;
        }
    }

    printf("bestmove %s\n", str_move(pv.moves[0], board->stm));
}
