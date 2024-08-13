#include "board.h"
#include "hash.h"
#include "uci.h"

int main()
{
    init_hash();
    struct board_t board = {
        .bb = {0}, .stm = WHITE, .ep = NOSQ, .ca = 0, .hist = {0}};
    parse(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    loop(&board);
    return 0;
}
