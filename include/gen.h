#ifndef GEN_H
#define GEN_H

#include "board.h"
#include <stdint.h>

/**
 * Quiet Moves.
 * Generates moves that are not checks or captures.
 */
int quiet(const struct board_t *board, uint16_t *moves);

/**
 * Noisy Moves.
 * Generate moves that are captures or checks.
 */
int noisy(const struct board_t *board, uint16_t *moves);

/**
 * Legal Move.
 * Check if a given move is legal in the given board.
 */
uint64_t legal(const struct board_t *board, uint16_t move);

#endif /* GEN_H */
