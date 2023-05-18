#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

#define EMPTY 0ULL                // empty bitboard
#define FULL  0xFFFFFFFFFFFFFFFF  // full bitboard
#define R2    0x000000000000FF00  // 2nd rank
#define R4    0x00000000FF000000  // 4th rank
#define R5    0x000000FF00000000  // 5th rank
#define R7    0x00FF000000000000  // 7th rank
#define FA    0x0101010101010101  // A file
#define FH    0x8080808080808080  // H file

#define set(b, i) (b |= 1ULL << i)
#define unset(b, i) (b &= ~(1ULL << i))
#define toggle(b, i) (b ^= 1ULL << i)

/** @brief count the number of bits in a 64bit integer
 *  @param b a bitset (bitboard)
 *  @return int
 */
int popcnt(uint64_t b);

/** @brief bit scan forward returns the index of the least significant 1 bit
 *  @param b a bitset (bitboard)
 *  @return int
 */
int bsf(uint64_t b);

/** @brief reverse a bitboard 
 *  @param b a bitset (bitboard)
 *  @return uint64_t
 */
uint64_t rev(uint64_t b);

/** @brief Hyperbola Quintessence's o^(o - 2r) trick
 *  @param b a bitset (bitboard)
 *  @return uint64_t
 */
uint64_t oo2r(int i, uint64_t block, uint64_t mask);

void printb(uint64_t b);

#endif /* BITBOARD_H */
