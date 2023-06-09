#ifndef GEN_H
#define GEN_H

#include "board.h"
#include <stdint.h>

/** @brief generate non-capture moves in the position
 *  @param board board state
 *  @param moves a list of legal moves
 *  @return void
 */
void quiet(const struct board_t *board, uint16_t *moves);

/** @brief generate capture moves in the position
 *  @param board board state
 *  @param moves a list of legal moves
 *  @return void
 */
void noisy(const struct board_t *board, uint16_t *moves)

#endif /* GEN_H */
