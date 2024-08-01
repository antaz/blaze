#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"

#define MOVESTOGO 24
#define MAX_DEPTH 64

struct clck_t {
    int time[2];
    int inc[2];
    int depth;
    int nodes;
    int movetime;
    int movestogo;
};

void deepen(struct board_t *board, struct clck_t *clock);
int search(struct board_t *board, int depth, uint16_t *pv);

#endif
