#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

#define set(bb, i) (bb |= 1ULL << i)
#define unset(bb, i) (bb &= ~(1ULL << i))
#define toggle(bb, i) (bb ^= 1ULL << i)
#define rank(i) (i & 7)
#define file(i) (i >> 3)
#define index(f, r) (8 * r + f)

/* print bitboard */
void printb(uint64_t bb);

#endif /* BITBOARD_H */
