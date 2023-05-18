#include <stdio.h>
#include <stdlib.h>

#include "bitboard.h"
#include "board.h"
#include "type.h"
#include "gen.h"

int main()
{
        const char *start = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
        struct board_t board = { .bb = { 0 }, .mail = { NOPC },
				 .turn = WHITE, .ep = NOSQ, .ca = WK | WQ };

        parse(start, &board);

        uint16_t *moves = (uint16_t *) calloc(200, sizeof(uint16_t));
        return 0;
}
