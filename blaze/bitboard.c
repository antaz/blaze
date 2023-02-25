#include "bitboard.h"

#include <stdint.h>
#include <stdio.h>

void printb(uint64_t bb)
{
        int i;

        for(i = 0; i < 64; ++i)
                if ((i+1) % 8)
                        bb & (1ULL << i) ? printf("X ") : printf(". ");
                else
                        bb & (1ULL << i) ? printf("X\n") : printf(".\n");
}
