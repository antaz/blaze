#include "move.h"
#include "board.h"
#include <stdint.h>
#include <stdio.h>

char *str_move(uint16_t move, int turn)
{
    // TODO: refactor and simplify this
    static char buffer[6];
    int from = MOVE_FROM(move);
    int to = MOVE_TO(move);
    int type = MOVE_TYPE(move);
    char prom = 0;

    if (turn == BLACK) {
        from = from ^ 0x38;
        to = to ^ 0x38;
    }

    switch (type) {
    case NPC:
    case NP:
        prom = 'n';
        break;
    case BPC:
    case BP:
        prom = 'b';
        break;
    case RPC:
    case RP:
        prom = 'r';
        break;
    case QPC:
    case QP:
        prom = 'q';
        break;
    }

    if (prom != 0) {
        sprintf(buffer, "%c%d%c%d%c", ('a' + (from & 7)), (from >> 3) + 1,
                ('a' + (to & 7)), (to >> 3) + 1, prom);
    } else {
        sprintf(buffer, "%c%d%c%d", ('a' + (from & 7)), (from >> 3) + 1,
                ('a' + (to & 7)), (to >> 3) + 1);
    }
    return buffer;
}

uint16_t parse_move(char *san)
{
    int from, to;

    from = 8 * (san[0] - 'a');
    from += ((san[1] - '0') - 1);
    to = 8 * (san[2] - 'a');
    to += ((san[3] - '0') - 1);

    return MOVE(from, to, 0);
}
