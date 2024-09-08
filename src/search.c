#include "search.h"
#include "board.h"
#include "eval.h"
#include "gen.h"
#include "move.h"
#include "uci.h"
#include <inttypes.h>
#include <search.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static struct search_t driver;
static uint64_t movetime; // how long should we search

static void halt()
{
    // check if we should stop searching
    if (movetime) {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        uint64_t elapsed = (now.tv_sec - driver.start.tv_sec) * 1e3 +
                           (now.tv_nsec - driver.start.tv_nsec) / 1e6;
        if (elapsed >= movetime) {
            driver.stop = 1;
        }
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

static int search(struct board_t *board, int alpha, int beta, int depth,
                  struct pv_t *pv)
{
    int score = 0;
    struct pv_t cpv;
    cpv.count = 0;

    if ((driver.nodes & 1023) == 0)
        halt();

    if (depth == 0) {
        pv->count = 0;
        return eval(board);
    }

    driver.nodes++;

    if (board->ply && rep(board)) {
        pv->count = 0;
        return 0;
    }

    struct move_t moves[256];
    int count = gen_legal(board, moves);

    for (int i = 0; i < count; i++) {
        make(board, moves[i].data);
        score = -search(board, -beta, -alpha, depth - 1, &cpv);
        take(board, moves[i].data);

        if (driver.stop)
            break;

        if (score > alpha) {
            if (score >= beta)
                return beta;
            alpha = score;
            pv->moves[0] = moves[i].data;
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
            return -INF + board->ply;
        } else {
            return 0;
        }
    }

    return alpha;
}

void deepen(struct board_t *board)
{
    int score;
    int alpha = -2 * INF, beta = 2 * INF;
    int max_depth = MAX_DEPTH;
    uint16_t bestmove = 0;
    struct pv_t pv = {0};
    driver.stm = board->stm;
    driver.stop = 0;
    driver.nodes = 0;
    board->ply = 0;

    // initialize time management
    clock_gettime(CLOCK_MONOTONIC, &driver.start);

    if (!tc_data.movetime) {
        movetime = tc_data.time[board->stm] / tc_data.movestogo +
                   tc_data.inc[board->stm];
    } else {
        movetime = tc_data.movetime;
    }

    if (tc_data.depth)
        max_depth = tc_data.depth;

    // iterative deepening
    for (driver.depth = 1; driver.depth <= max_depth; driver.depth++) {

        score = search(board, alpha, beta, driver.depth, &pv);

        if (driver.stop)
            break;

        bestmove = pv.moves[0];
        pvinfo(&driver, &pv, score);
    }

    printf("bestmove %s\n", m2uci(bestmove, driver.stm));
}
