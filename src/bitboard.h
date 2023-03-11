#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdbool.h>
#include <stdint.h>

#define set(b, i) (b |= 1ULL << i)
#define unset(b, i) (b &= ~(1ULL << i))
#define toggle(b, i) (b ^= 1ULL << i)
#define rank(i) (i >> 3)
#define file(i) (i & 7)
#define diag(i) 7 + rank(i) - file(i)
#define adiag(i) rank(i) + file(i)
#define index(f, r) (8 * r + f)

/* popcount: Population Count the number of 1 bits
 * Brian Kernighan's way
 */
int popcount(uint64_t bb);

/* bsf: Bit Scan Forward to find the index of the least significant 1 bit
 * Using population count
 */
int bsf(uint64_t bb);

/* king attack vector for a given square index */
uint64_t kattack(int i);

/* knight attack vector for a given square index */
uint64_t nattack(int i);

/* pawn attack vector for a given square index and color */
uint64_t pattack(int i, bool color);

/* bishop attack vector from a given square index and a block set */
uint64_t battack(int i, uint64_t block);

/* rook attack vector from a given square index and a block set */
uint64_t rattack(int i, uint64_t block);

uint64_t between(int i, int j);

uint64_t line(int i, int j);

/* print bitboard */
void printb(uint64_t bb);

#endif /* BITBOARD_H */
