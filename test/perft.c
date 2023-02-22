#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"

static uint64_t perft(struct board_t *board, int depth)
{
        uint8_t i;
        uint64_t nodes = 0;

        MoveList list[1];
        generateMoves(board, list);

        if (depth == 0)
                return 1;

        for (i = 0; i < list->count; ++i) {
                if (!makeMove(board, list->moves[i]))
                        continue;
                nodes += perft(board, depth - 1);
                takeMove(board);
        }

        return nodes;
}

int main(int argc, char *argv[])
{
        int i;
        struct board_t board[1];
        printf("%s\n", argv[1]);
        parseFEN(argv[1], board);
        for (i = 1; i <= atoi(argv[2]); ++i)
                printf("%-8d%lu\n", i, perft(board, i));
}
