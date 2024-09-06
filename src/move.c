#include "move.h"
#include "board.h"
#include <stdint.h>
#include <stdio.h>

char *m2uci(uint16_t m, int stm)
{
    static char buf[6];

    int from = MOVE_FROM(m) ^ (stm == BLACK ? 0x38 : 0);
    int to = MOVE_TO(m) ^ (stm == BLACK ? 0x38 : 0);
    char prom = "\0\0\0nbrq"[MOVE_TYPE(m) & 7];

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
