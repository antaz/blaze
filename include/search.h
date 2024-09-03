#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"

#define MOVESTOGO 24
#define MAX_DEPTH 64

// principal variation
struct pv_t {
    uint16_t moves[MAX_DEPTH];
    int count;
};

// time control information
extern struct tc_t {
    uint64_t time[2]; // time for both players
    uint64_t inc[2];  // increment for both players
    int depth;
    uint64_t nodes;
    uint64_t movetime;
    int movestogo;
} tc_data;

void go(struct board_t *board);

#endif
