#include "hash.h"
#include "board.h"
#include <stdint.h>
#include <stdio.h>

static uint64_t prng()
{
    static uint64_t state = 0x123456789ABCDEF;
    state ^= state >> 12;
    state ^= state << 25;
    state ^= state >> 27;
    return state * 0x2545F4914F6CDD1D;
}

void init_hash()
{
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < PC; j++) {
            for (int k = 0; k < SQ; k++) {
                piece_hash[i][j][k] = prng();
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        ep_hash[i] = prng();
    }

    for (int i = 0; i < 52; i++) {
        ca_hash[i] = prng();
    }

    stm_hash = prng();
}
