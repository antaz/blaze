#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

#define set(bb, i) (bb |= 1ULL << i)
#define unset(bb, i) (bb &= ~(1ULL << i))
#define toggle(bb, i) (bb ^= 1ULL << i)
#define rank(i) (i & 7)
#define file(i) (i >> 3)
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

/* white pawn attack vector for a given square index */
uint64_t wpattack(int i);

/* black pawn attack vector for a given square index */
uint64_t bpattack(int i);

/* print bitboard */
void printb(uint64_t bb);

#endif /* BITBOARD_H */
