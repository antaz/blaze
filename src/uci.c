#include "uci.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "perft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 65536

void loop(struct board_t *board)
{
    char buf[SIZE];

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    for (;;) {
        if (!fgets(buf, SIZE, stdin)) {
            continue;
        }
        if (!strncmp("isready", buf, 7)) {
            printf("readyok\n");
        } else if (!strncmp("go perft", buf, 8)) {
            perft_divide(board, atoi(buf + 9));
        } else if (!strncmp("ucinewgame", buf, 10)) {
            parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                  board);
        } else if (!strncmp("uci", buf, 3)) {
            printf("id name Blaze 0.1\n");
            printf("id author Antar Azri\n");
            printf("uciok\n");
        } else if (!strncmp("position", buf, 8)) {
            if (strstr(buf, "startpos")) {
                parse("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq "
                      "- 0 1",
                      board);
            } else if (strstr(buf, "fen")) {
                parse(buf + 13, board);
            }

            char *moves = strstr(buf, "moves");
            if (moves) {
                moves += 6;
                while ((moves[0] >= 'a') && (moves[0] <= 'h')) {
                    int from, to;
                    uint16_t m = 0;

                    from = moves[0] - 'a';
                    from += 8 * ((moves[1] - '0') - 1);
                    to = moves[2] - 'a';
                    to += 8 * ((moves[3] - '0') - 1);

                    if(board->turn == BLACK) {
                        m = MOVE((from ^ 0x38), (to ^ 0x38), 0);
                    } else {
                        m = MOVE(from, to, 0);
                    }

                    // Make move
                    make(board, m);

                    moves += 4;
                    while (moves[0] == ' ')
                        moves++;
                }
            }
        } else if (!strncmp("go", buf, 2)) {
            uint16_t moves[320];
            int count = 0;
            count = quiet(board, moves);
            count += noisy(board, moves);

            for (int i = 0; i < count; ++i) {
                if (legal(board, moves[i])) {
                    continue;
                }
                printf("bestmove %s\n", str_move(moves[0], board->turn));
                break;
            }
        } else if (!strncmp("stop", buf, 4)) {
            // stop search
        } else if (!strncmp("quit", buf, 4)) {
            break;
        }
    }
}
