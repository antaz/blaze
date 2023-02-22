#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "search.h"
#include "uci.h"
#include "makemove.h"

#define NAME "Blaze"
#define AUTHOR "Dr. Sara Tancredi"
#define INPUTBUFFER 2400

void go(struct board_t *board, Search *s, char *line)
{
        PV pv;
        pv.count = 0;
        s->nodes = 0;
        s->stop = 0;

        int depth = -1, movestogo = 40, movetime = -1;
        int time = -1, inc = 0;
        char *ptr = NULL;
        s->timeset = 0;

        if ((ptr = strstr(line, "binc")) && board->turn == BLACK)
                inc = atoi(ptr + 5);
        if ((ptr = strstr(line, "winc")) && board->turn == WHITE)
                inc = atoi(ptr + 5);
        if ((ptr = strstr(line, "wtime")) && board->turn == WHITE)
                time = atoi(ptr + 6);
        if ((ptr = strstr(line, "btime")) && board->turn == BLACK)
                time = atoi(ptr + 6);
        if ((ptr = strstr(line, "movestogo")))
                movestogo = atoi(ptr + 10);
        if ((ptr = strstr(line, "movetime")))
                movetime = atoi(ptr + 9);

        if ((ptr = strstr(line, "depth")))
                depth = atoi(ptr + 6);

        if (movetime != -1) {
                time = movetime;
                movestogo = 1;
        }

        s->starttime = current_timestamp();
        s->depth = depth;

        if (time != -1) {
                s->timeset = 1;
                time /= movestogo;
                time -= 20;
                s->stoptime = s->starttime + time + inc;
        }

        if (depth == -1)
                s->depth = MAXDEPTH;

        search(board, s, &pv);
}

void position(char *lineIn, struct board_t *board)
{
        char *ptrChar = lineIn + 9;
        struct move_t move;

        lineIn = ptrChar;

        if (strncmp(lineIn, "startpos", 8) == 0) {
                parseFEN(START_FEN, board);
        } else {
                ptrChar = strstr(lineIn, "fen");
                if (ptrChar == NULL) {
                        parseFEN(START_FEN, board);
                } else {
                        ptrChar += 4;
                        parseFEN(ptrChar, board);
                }
        }

        ptrChar = strstr(lineIn, "moves");

        if (ptrChar != NULL) {
                ptrChar += 6;
                while (*ptrChar) {
                        if (!parseMove(board, ptrChar, &move))
                                break;
                        makeMove(board, move);
                        board->ply = 0;
                        while (*ptrChar && *ptrChar != ' ')
                                ptrChar++;
                        ptrChar++;
                }
        }
        // printBoardSAN(board);
}
void uci_loop(struct board_t *board, Search *search)
{

        setbuf(stdin, NULL);
        setbuf(stdout, NULL);

        char line[INPUTBUFFER];
        while (1) {
                memset(&line[0], 0, sizeof(line));
                fflush(stdout);
                if (!fgets(line, INPUTBUFFER, stdin))
                        continue;
                if (line[0] == '\n')
                        continue;
                if (!strncmp(line, "isready", 7)) {
                        printf("readyok\n");
                        continue;
                } else if (!strncmp(line, "position", 8)) {
                        position(line, board);
                } else if (!strncmp(line, "ucinewgame", 10)) {
                        position("position startpos\n", board);
                } else if (!strncmp(line, "go", 2)) {
                        go(board, search, line);
                } else if (!strncmp(line, "quit", 4)) {
                        break;
                } else if (!(strncmp(line, "stop", 4))) {
                        search->stop = 1;
                } else if (!strncmp(line, "uci", 3)) {
                        printf("id name %s\n", NAME);
                        printf("id author %s\n", AUTHOR);
                        printf("uciok\n");
                }
        }
}
