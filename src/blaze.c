#include "board.h"
#include "hash.h"
#include "uci.h"
#include <stdint.h>
#include <stdlib.h>

int main()
{
    init_hash();
    init_table(2 * 1024 * 1024);
    struct board_t board = {
        .bb = {0}, .stm = WHITE, .ep = NOSQ, .ca = 0, .hist = {0}};
    parse(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    loop(&board);
    free(table);
    return 0;
}
