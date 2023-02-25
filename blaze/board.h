#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/* main structure to hold the state of the board */
struct board_t {
        uint64_t piece[8];
};

/* parse: parses `fen` (Forsyth–Edwards Notation) string into `board` structure */
void parse(const char *fen, struct board_t *board);

#endif /* BOARD_H */
