#include "perft.h"
#include "gen.h"
#include "move.h"
#include <stdio.h>
#include <time.h>

uint64_t perft(struct board_t *board, int depth)
{
    uint16_t moves[320];
    int count = 0;
    uint64_t nodes = 0;

    if (depth == 0)
        return 1ULL;

    count = gen_legal(board, moves);

    for (int i = 0; i < count; ++i) {
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
    clock_t start_time, end_time;
    double elapsed_time;

    count = gen_legal(board, moves);
    start_time = clock();

    for (int i = 0; i < count; ++i) {
        make(board, moves[i]);
        nodes = perft(board, depth - 1);
        unmake(board, moves[i]);
        printf("%s: %lu\n", str_move(moves[i], board->turn), nodes);
        total_nodes += nodes;
    }

    end_time = clock();
    elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    double nps = total_nodes / elapsed_time;

    printf("\nNodes searched: %lu\n", total_nodes);
    printf("NPS: %.2f\n", nps);
}
