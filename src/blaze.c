#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "attack.h"
#include "bitboard.h"
#include "board.h"
#include "gen.h"
#include "perft.h"
#include "type.h"

int main()
{
        const char *start = "r1bqk1nr/pppp1ppp/2n5/4p1B1/1b1P4/2N2N2/PPP1PPPP/"
                            "R2QKB1R b KQkq - 5 4";
        struct board_t board = {.bb = {0}, .turn = WHITE, .ep = NOSQ, .ca = 0};

        parse(start, &board);
        uint64_t total = perft(&board, 1);
        printf("%ld\n", total);
}
