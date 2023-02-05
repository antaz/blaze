#include "definitions.h"
#include "functions.h"
#include <stdio.h>

int main()
{
        char *start_fen =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        Board board[1];
        Search s[1];
        initZobrist();
        board->table->table = NULL;
        parseFEN(start_fen, board);
        initTTable(board->table);
        clearTTable(board->table);

        uci_loop(board, s);
        return 0;
}
