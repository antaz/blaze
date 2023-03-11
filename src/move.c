#include "move.h"

#include <stdint.h>
#include <stdio.h>

uint16_t move(int from, int to, int flags)
{
        return ((flags & 0xf) << 12) | ((from & 0x3f) << 6) | (to & 0x3f);
}

void printm(uint16_t m)
{
        if (m)
                printf("{ %d, %d, %d }", (m >> 6) & 0x3f, m & 0x3f,
                       (m >> 12) & 0x3f);
        else
                printf("{ empty }");
}
