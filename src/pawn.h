#ifndef PAWN_H
#define PAWN_H

#include <stdint.h>

/** @brief pawn attacks from square
 *  @param i square index
 *  @param color pawn color
 *  @return uint64_t
 */
uint64_t patk(const int i, const int color);

/** @brief pawn attacks from bitset
 *  @param b bitset of pawns
 *  @param color pawn color
 *  @return uint64_t
 */
uint64_t patks(const uint64_t b, const int color);

/** @brief single pawn push
 *  @param b bitset of pawns
 *  @param color pawn color
 *  @return uint64_t
 */
uint64_t push(const uint64_t b, const int color);

#endif /* PAWN_H */
