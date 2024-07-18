#include "perft.h"
#include "bitboard.h"
#include "gen.h"
#include "type.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_move(struct board_t *board, uint16_t move)
{
    int from = MOVE_FROM(move);
    int to = MOVE_TO(move);

    // from = from ^ 0x38;
    // to = to ^ 0x38;

    printf("%c%d%c%d", ('a' + (from & 7)), (from >> 3) + 1, ('a' + (to & 7)),
           (to >> 3) + 1);
}

uint64_t perft(struct board_t *board, int depth)
{
    uint16_t quiets[256];
    uint16_t noisies[64];
    int count = 0;
    uint64_t nodes = 0;

    if (depth == 0)
        return 1ULL;

    count = quiet(board, quiets);

    for (int i = 0; i < count; ++i) {
        // print_move(board, quiets[i]);
        // printf("\n");
        if (legal(board, quiets[i]))
            continue;
        // printf("%*s", i, "");
        if (depth > 1) {
            make(board, quiets[i]);
            nodes += perft(board, depth - 1);
            unmake(board, quiets[i]);
        } else {
            nodes++;
        }
    }

    count = noisy(board, noisies);

    for (int i = 0; i < count; ++i) {
        // print_move(board, noisies[i]);
        // printf("\n");
        if (legal(board, noisies[i]))
            continue;
        // printf("%*s", i, "");
        if (depth > 1) {
            make(board, noisies[i]);
            nodes += perft(board, depth - 1);
            unmake(board, noisies[i]);
        } else {
            nodes++;
        }
    }

    return nodes;
}

void perft_divide(struct board_t *board, int depth)
{
    uint16_t quiets[256];
    uint16_t noisies[64];
    int count = 0;
    uint64_t nodes = 0;
    uint64_t total_nodes = 0;

    count = quiet(board, quiets);

    for (int i = 0; i < count; ++i) {
        if (legal(board, quiets[i]))
            continue;
        make(board, quiets[i]);
        nodes = perft(board, depth - 1);
        unmake(board, quiets[i]);
        print_move(board, quiets[i]);
        printf(": %lu\n", nodes);
        total_nodes += nodes;
    }

    count = noisy(board, noisies);

    for (int i = 0; i < count; ++i) {
        if (legal(board, noisies[i]))
            continue;
        make(board, noisies[i]);
        nodes = perft(board, depth - 1);
        unmake(board, noisies[i]);
        print_move(board, noisies[i]);
        printf(": %lu\n", nodes);
        total_nodes += nodes;
    }

    printf("Total nodes: %lu\n", total_nodes);
}
