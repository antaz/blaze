#include "bitboard.h"
#include <stdint.h>

static const uint64_t PATK[2][64] = {{0x200,
                                      0x500,
                                      0xa00,
                                      0x1400,
                                      0x2800,
                                      0x5000,
                                      0xa000,
                                      0x4000,
                                      0x20000,
                                      0x50000,
                                      0xa0000,
                                      0x140000,
                                      0x280000,
                                      0x500000,
                                      0xa00000,
                                      0x400000,
                                      0x2000000,
                                      0x5000000,
                                      0xa000000,
                                      0x14000000,
                                      0x28000000,
                                      0x50000000,
                                      0xa0000000,
                                      0x40000000,
                                      0x200000000,
                                      0x500000000,
                                      0xa00000000,
                                      0x1400000000,
                                      0x2800000000,
                                      0x5000000000,
                                      0xa000000000,
                                      0x4000000000,
                                      0x20000000000,
                                      0x50000000000,
                                      0xa0000000000,
                                      0x140000000000,
                                      0x280000000000,
                                      0x500000000000,
                                      0xa00000000000,
                                      0x400000000000,
                                      0x2000000000000,
                                      0x5000000000000,
                                      0xa000000000000,
                                      0x14000000000000,
                                      0x28000000000000,
                                      0x50000000000000,
                                      0xa0000000000000,
                                      0x40000000000000,
                                      0x200000000000000,
                                      0x500000000000000,
                                      0xa00000000000000,
                                      0x1400000000000000,
                                      0x2800000000000000,
                                      0x5000000000000000,
                                      0xa000000000000000,
                                      0x4000000000000000,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0},

                                     {0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x0,
                                      0x2,
                                      0x5,
                                      0xa,
                                      0x14,
                                      0x28,
                                      0x50,
                                      0xa0,
                                      0x40,
                                      0x200,
                                      0x500,
                                      0xa00,
                                      0x1400,
                                      0x2800,
                                      0x5000,
                                      0xa000,
                                      0x4000,
                                      0x20000,
                                      0x50000,
                                      0xa0000,
                                      0x140000,
                                      0x280000,
                                      0x500000,
                                      0xa00000,
                                      0x400000,
                                      0x2000000,
                                      0x5000000,
                                      0xa000000,
                                      0x14000000,
                                      0x28000000,
                                      0x50000000,
                                      0xa0000000,
                                      0x40000000,
                                      0x200000000,
                                      0x500000000,
                                      0xa00000000,
                                      0x1400000000,
                                      0x2800000000,
                                      0x5000000000,
                                      0xa000000000,
                                      0x4000000000,
                                      0x20000000000,
                                      0x50000000000,
                                      0xa0000000000,
                                      0x140000000000,
                                      0x280000000000,
                                      0x500000000000,
                                      0xa00000000000,
                                      0x400000000000,
                                      0x2000000000000,
                                      0x5000000000000,
                                      0xa000000000000,
                                      0x14000000000000,
                                      0x28000000000000,
                                      0x50000000000000,
                                      0xa0000000000000,
                                      0x40000000000000}

};

uint64_t patk(const int i, int color) { return PATK[color][i]; }

uint64_t patks(const uint64_t b, const int color)
{
        return color ? ((b << 7) & ~FH) | ((b << 9) & ~FA)
                     : ((b >> 7) & ~FA) | ((b >> 9) & ~FH);
}

uint64_t push(const uint64_t b, const int color)
{
        // rotate is probably faster!
        return ((b << 8) >> (color << 4));
}