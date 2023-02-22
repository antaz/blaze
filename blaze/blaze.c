#include <stdio.h>

#include "board.h"
#include "search.h"
#include "hash.h"
#include "uci.h"

int main()
{
        char *start_fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        struct board_t board[1];
        Search s[1];
        initZobrist();
        parseFEN(start_fen, board);

        uci_loop(board, s);
        return 0;
}
