#include "bitboard.h"
#include <stdio.h>

uint64_t rot180(uint64_t b)
{
        b = (b & 0x5555555555555555) << 1 | ((b >> 1) & 0x5555555555555555);
        b = (b & 0x3333333333333333) << 2 | ((b >> 2) & 0x3333333333333333);
        b = (b & 0x0f0f0f0f0f0f0f0f) << 4 | ((b >> 4) & 0x0f0f0f0f0f0f0f0f);
        b = (b & 0x00ff00ff00ff00ff) << 8 | ((b >> 8) & 0x00ff00ff00ff00ff);

        return (b << 48) | ((b & 0xffff0000) << 16) | ((b >> 16) & 0xffff0000) |
               (b >> 48);
}

uint64_t rev(uint64_t b)
{
        b = ((b >> 8) & 0x00FF00FF00FF00FF) | ((b & 0x00FF00FF00FF00FF) << 8);
        b = ((b >> 16) & 0x0000FFFF0000FFFF) | ((b & 0x0000FFFF0000FFFF) << 16);
        b = (b >> 32) | (b << 32);
        return b;
}

uint64_t oo2r(int i, uint64_t block, uint64_t mask)
{
        return (((mask & block) - (1ULL << i) * 2) ^
                rot180(rot180(mask & block) - rot180(1ULL << i) * 2)) &
               mask;
}

int popcnt(uint64_t b)
{
        int c = 0;

        while (b) {
                ++c;
                b &= b - 1;
        }
        return c;
}

int bsf(uint64_t b) { return popcnt((b & -b) - 1); }

void printb(uint64_t b)
{
        for (int r = 7; r >= 0; --r) {
                for (int f = 0; f < 8; ++f) {
                        b & (1ULL << (r * 8 + f)) ? printf("X ") : printf(". ");
                }
                printf("\n");
        }
}
