#ifndef GEN_H
#define GEN_H

#include "board.h"
#include "move.h"
#include <stdint.h>

/**
 * Quiet Moves.
 * Generates moves that are not checks or captures.
 */
int quiet(const struct board_t *board, struct move_t *moves);

/**
 * Noisy Moves.
 * Generate moves that are captures or checks.
 */
int noisy(const struct board_t *board, struct move_t *moves);

/**
 * Legal Move.
 * Check if a given move is legal in the given board.
 */
uint64_t legal(const struct board_t *board, uint16_t move);
uint64_t check(const struct board_t *board);

int gen(const struct board_t *board, struct move_t *moves);
int gen_legal(const struct board_t *board, struct move_t *moves);
int capture(const struct board_t *board, struct move_t *moves);

#endif /* GEN_H */
