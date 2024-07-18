#include "uci.h"
#include "board.h"
#include "perft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024

void loop(struct board_t *board)
{
    char buf[1024];

    while (1) {
        if (fgets(buf, 1024, stdin) != NULL) {
            if (strcmp("uci\n", buf) == 0) {
                printf("uciok\n");
            } else if (strcmp("ready\n", buf) == 0) {
                printf("readyok\n");
            } else if (strncmp("go perft", buf, 7) == 0) {
                perft_divide(board, atoi(buf + 9));
            } else if (strncmp("position startpos", buf, 16) == 0) {
                parse(
                    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                    board);
            } else if (strncmp("position fen", buf, 11) == 0) {
                parse(buf + 13, board);
            }
        }
    }
}
