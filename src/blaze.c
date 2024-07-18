#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "attack.h"
#include "bitboard.h"
#include "board.h"
#include "gen.h"
#include "perft.h"
#include "type.h"

int main()
{
    struct board_t board = {
        .bb = {0}, .turn = BLACK, .ep = NOSQ, .ca = 0, .hist = {0}};
    const char *start =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    parse(start, &board);
    perft_divide(&board, 6);
}
