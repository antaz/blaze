#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

#define EMPTY 0ULL		// empty bitboard
#define FULL 0xFFFFFFFFFFFFFFFF // full bitboard
#define R2 0x000000000000FF00	// 2nd rank
#define R4 0x00000000FF000000	// 4th rank
#define R5 0x000000FF00000000	// 5th rank
#define R7 0x00FF000000000000	// 7th rank
#define FA 0x0101010101010101	// A file
#define FH 0x8080808080808080	// H file

#define set(b, i) (b |= 1ULL << i)
#define unset(b, i) (b &= ~(1ULL << i))
#define toggle(b, i) (b ^= 1ULL << i)
#define LSB(b) ((b) & (-(signed long long)(b)))
#define MSB(b) (0x3F ^ __builtin_clzll(b))

/**
 * Population Count.
 * Counts the number of set bits in a bitboard.
 */
int popcnt(uint64_t b);

/**
 * Bit Scan Forward.
 * Find the least significant set bit.
 */
int bsf(uint64_t b);

/**
 * Flip a bitboard about the center.
 * Rank 1 is Rank 8 and vice versa.
 */
uint64_t vflip(uint64_t b);

/**
 * Reverse bitboard
 * Square A1 is mapped to H8, A2 to H7 etc
 */
uint64_t rev(uint64_t b);

/**
 * Hyperbola Quintessence o^(o - 2r) trick.
 */
uint64_t oo2r(int i, uint64_t block, uint64_t mask);

/**
 * Print a bitboard.
 */
void printb(uint64_t b);

#endif /* BITBOARD_H */
