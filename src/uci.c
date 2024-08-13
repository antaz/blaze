#include "uci.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "perft.h"
#include "search.h"
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 65536
struct tc_t tc_data = {.movestogo = MOVESTOGO};

void loop(struct board_t *board)
{
    char buf[BUF_SIZE];

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    for (;;) {
        if (!fgets(buf, BUF_SIZE, stdin)) {
            break;
        }

        if (!strncmp("isready", buf, 7)) {
            printf("readyok\n");
        } else if (!strncmp("go perft", buf, 8)) {
            perft_divide(board, atoi(buf + 9));
        } else if (!strncmp("ucinewgame", buf, 10)) {
            parse(board,
                  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        } else if (!strncmp("uci", buf, 3)) {
            printf("id name Blaze 0.1\n");
            printf("id author Antar Azri\n");
            printf("uciok\n");
        } else if (!strncmp("position", buf, 8)) {
            if (strstr(buf, "startpos")) {
                parse(board,
                      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq "
                      "- 0 1");
            } else if (strstr(buf, "fen")) {
                parse(board, buf + 13);
            }

            char *moves = strstr(buf, "moves");
            if (moves) {
                char *move = strtok(moves + 6, " \n");
                while (move != NULL) {
                    uint16_t moves[320];
                    int count = 0;
                    count = gen_legal(board, moves);
                    char *str_m;

                    for (int i = 0; i < count; ++i) {
                        str_m = str_move(moves[i], board->stm);
                        if (!strcmp(str_m, move)) {
                            make(board, moves[i]);
                            break;
                        }
                    }

                    move = strtok(NULL, " \n");
                }
            }
        } else if (!strncmp("go", buf, 2)) {
            char *token = NULL;

            if ((token = strstr(buf, "infinite"))) {
                // do an infinite search
            }
            if ((token = strstr(buf, "ponder"))) {
                // ponder a move
            }
            if ((token = strstr(buf, "wtime"))) {
                tc_data.time[WHITE] = atoi(token + 6);
            }
            if ((token = strstr(buf, "btime"))) {
                tc_data.time[BLACK] = atoi(token + 6);
            }
            if ((token = strstr(buf, "winc"))) {
                tc_data.time[WHITE] = atoi(token + 5);
            }
            if ((token = strstr(buf, "binc"))) {
                tc_data.time[BLACK] = atoi(token + 5);
            }
            if ((token = strstr(buf, "movestogo"))) {
                tc_data.movestogo = atoi(token + 10);
            }
            if ((token = strstr(buf, "depth"))) {
                tc_data.depth = atoi(token + 6);
            }
            if ((token = strstr(buf, "nodes"))) {
                tc_data.nodes = atoi(token + 6);
            }
            if ((token = strstr(buf, "movetime"))) {
                tc_data.movetime = atoi(token + 9);
            }

            go(board);
        } else if (!strncmp("stop", buf, 4)) {
            // stop search
        } else if (!strncmp("quit", buf, 4)) {
            break;
        }
    }
}
