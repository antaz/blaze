#include "perft.h"
#include "gen.h"
#include "move.h"
#include <stdio.h>

uint64_t perft(struct board_t *board, int depth)
{
    uint16_t moves[320];
    int count = 0;
    uint64_t nodes = 0;

    if (depth == 0)
        return 1ULL;

    count = quiet(board, moves);
    count += noisy(board, moves + count);

    for (int i = 0; i < count; ++i) {
        if (legal(board, moves[i]))
            continue;
        if (depth > 1) {
            make(board, moves[i]);
            nodes += perft(board, depth - 1);
            unmake(board, moves[i]);
        } else {
            nodes++;
        }
    }

    return nodes;
}

void perft_divide(struct board_t *board, int depth)
{
    uint16_t moves[320];
    int count = 0;
    uint64_t nodes = 0;
    uint64_t total_nodes = 0;

    count = quiet(board, moves);
    count += noisy(board, moves + count);

    for (int i = 0; i < count; ++i) {
        if (legal(board, moves[i]))
            continue;
        make(board, moves[i]);
        nodes = perft(board, depth - 1);
        unmake(board, moves[i]);
        printf("%s: %lu\n", str_move(moves[i], board->turn), nodes);
        total_nodes += nodes;
    }

    printf("\nNodes searched: %lu\n", total_nodes);
}
