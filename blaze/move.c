#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "move.h"

void printMove(struct move_t move)
{
        char *pieces = ".pnbrq";
        int promoted = move.promoted;
        assert(move.from != OFFBOARD);
        assert(move.to != OFFBOARD);
        if (move.from == 0 && move.to == 0) {
                printf("0000");
                return;
        }
        if (promoted) {
                printf("%c%d%c%d%c", (FILE120(move.from)) + 'a',
                       RANK120(move.from) + 1, (FILE120(move.to)) + 'a',
                       RANK120(move.to) + 1, pieces[promoted % 6]);
        } else {
                printf("%c%d%c%d", (FILE120(move.from)) + 'a',
                       RANK120(move.from) + 1, (FILE120(move.to)) + 'a',
                       RANK120(move.to) + 1);
        }
}
