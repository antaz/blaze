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
    loop(&board);
}
