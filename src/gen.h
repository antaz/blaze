#ifndef GEN_H
#define GEN_H

#include "board.h"
#include <stdint.h>

/** @brief generate legal moves in the position
 *  @param board board state
 *  @param moves a list of legal moves
 *  @return void
 */
void generate(const struct board_t *board, uint16_t *moves);

void genwp(const uint64_t *bb, uint16_t *moves);
void genbp(const uint64_t *bb, uint16_t *moves);

/** @brief gives a bitboard checker pieces
 *  @param bb piece bitboards
 *  @param color side to move
 *  @return uint64_t
 */
uint64_t check(const uint64_t bb[], const int color);

/** @brief gives a bitboard of pinned pieces
 *  @param bb piece bitboards
 *  @param color side to move
 *  @return uint64_t
 */
uint64_t pin(const uint64_t bb[], const int color);

/** @brief gives a bitboard of squares attacked by op pieces
 *  @param bb piece bitboards
 *  @param color side to move
 *  @return uint64_t
 */
uint64_t danger(const uint64_t bb[], const int color);

#endif /* GEN_H */
