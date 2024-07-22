#include "board.h"
#include "uci.h"

int main()
{
    struct board_t board = {
        .bb = {0}, .turn = WHITE, .ep = NOSQ, .ca = 0, .hist = {0}};
    parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", &board);
    loop(&board);
    return 0;
}
