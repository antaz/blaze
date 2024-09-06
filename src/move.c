#include "move.h"
#include "board.h"
#include <stdint.h>
#include <stdio.h>

char *m2uci(uint16_t m, int stm)
{
    static char buf[6];
    int from = MOVE_FROM(m), to = MOVE_TO(m);

    if (stm == BLACK) {
        from ^= 0x38;
        to ^= 0x38;
    }

    char prom = "\0\0\0\0\0\0\0\0nbrknbrk"[MOVE_TYPE(m)];

    sprintf(buf, "%c%d%c%d%c", 'a' + (from & 7), (from >> 3) + 1,
            'a' + (to & 7), (to >> 3) + 1, prom);
    return buf;
}

uint16_t uci2m(char *uci)
{
    int from = 8 * (uci[0] - 'a') + (uci[1] - '1');
    int to = 8 * (uci[2] - 'a') + (uci[3] - '1');

    return MOVE(from, to, 0);
}
