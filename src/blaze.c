#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "board.h"
#include "type.h"
#include "uci.h"

int main()
{
    struct board_t board = {
        .bb = {0}, .turn = BLACK, .ep = NOSQ, .ca = 0, .hist = {0}};
    const char *start =
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";

    parse(start, &board);
    loop(&board);

}
