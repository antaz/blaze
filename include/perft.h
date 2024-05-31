#ifndef PERFT_H
#define PERFT_H

#include "board.h"
#include <stdint.h>

uint64_t perft(struct board_t *board, int depth);

#endif /* PERFT_H */