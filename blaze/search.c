#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "evaluation.h"
#include "makemove.h"

#define MATE 99999


// search driver
typedef struct {
        long long starttime, stoptime;
        int depth, timeset, movestogo, infinite, stop;
        long nodes;
} Search;

#ifdef WIN32
#include <Windows.h>
#include <io.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

long long current_timestamp()
{
#ifdef WIN32
        return GetTickCount();
#else
        struct timeval te;
        gettimeofday(&te, NULL);
        return te.tv_sec * 1000 + te.tv_usec / 1000;
#endif
}


int inputWaiting()
{
#ifndef WIN32
        struct timeval tv;
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(fileno(stdin), &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        select(16, &readfds, 0, 0, &tv);

        return (FD_ISSET(fileno(stdin), &readfds));
#else
        static int init = 0, pipe;
        static HANDLE inh;
        DWORD dw;

        if (!init) {
                init = 1;
                inh = GetStdHandle(STD_INPUT_HANDLE);
                pipe = !GetConsoleMode(inh, &dw);
                if (!pipe) {
                        SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT |
                                                   ENABLE_WINDOW_INPUT));
                        FlushConsoleInputBuffer(inh);
                }
        }
        if (pipe) {
                if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL))
                        return 1;
                return dw;
        } else {
                GetNumberOfConsoleInputEvents(inh, &dw);
                return dw <= 1 ? 0 : dw;
        }
#endif
}

void readInput(Search *search)
{
        int bytes;
        char input[256] = "", *endc;

        if (inputWaiting()) {
                search->stop = 1;
                do {
                        bytes = read(fileno(stdin), input, 256);
                } while (bytes < 0);
                endc = strchr(input, '\n');
                if (endc)
                        *endc = 0;

                if (strlen(input) > 0) {
                        if (!strncmp(input, "quit", 4)) {
                                // search->quit = 1;
                        }
                }
                return;
        }
}

static void checkTime(Search *search)
{
        if (search->timeset && current_timestamp() > search->stoptime)
                search->stop = 1;

        readInput(search);
}

static int isRep(struct board_t *board)
{
        for (int i = board->hisPly - board->fiftyMove; i < board->hisPly; ++i) {
                if (board->zobristHash == board->history[i].zobristHash)
                        return 1;
        }
        return 0;
}

static int qSearch(int alpha, int beta, struct board_t *board, Search *search)
{

        int movenum = 0;
        // int legal = 0;
        int score;

        if ((search->nodes & 0x3FF) == 0) {
                checkTime(search);
        }

        search->nodes++;

        MoveList list[1];
        score = evaluate(board);

        if (score >= beta)
                return beta;
        if (score > alpha)
                alpha = score;

        generateCaptures(board, list);
        score = -2 * MATE;

        for (movenum = 0; movenum < list->count; movenum++) {
                if (!makeMove(board, list->moves[movenum]))
                        continue;

                // legal++;
                score = -qSearch(-beta, -alpha, board, search);
                takeMove(board);

                if (search->stop)
                        return 0;

                if (score > alpha) {
                        if (score >= beta)
                                return beta;
                        alpha = score;
                }
        }
        return alpha;
}

static int alphaBeta(int alpha, int beta, int depth, struct board_t *board,
                     Search *search, PV *pv)
{

        int bestscore = -2 * MATE;
        int score = -2 * MATE;
        PV childpv;
        childpv.count = 0;
        int movenum = 0;
        int legal = 0;
        int check;

        if ((search->nodes & 0x3FF) == 0)
                checkTime(search);

        if (isRep(board)) {
                return 0;
        }
        search->nodes++;

        if (depth == 0) {
                pv->count = 0;
                return qSearch(alpha, beta, board, search);
        }

        MoveList list[1];
        generateMoves(board, list);

        check =
            isAttacked(board, board->kingSquare[board->turn], board->turn ^ 1);

        qsort(list->moves, list->count, sizeof(struct move_t), compareMoves);

        for (movenum = 0; movenum < list->count; movenum++) {

                if (!makeMove(board, list->moves[movenum])) {
                        continue;
                }

                legal++;
                score = -alphaBeta(-beta, -alpha, depth - 1, board, search,
                                   &childpv);
                takeMove(board);

                if (search->stop) {
                        return 0;
                }

                if (score > bestscore) {
                        bestscore = score;
                        if (score > alpha) {
                                if (score >= beta) {
                                        return beta;
                                }
                                alpha = score;
                                pv->moves[0] = list->moves[movenum];
                                memcpy(pv->moves + 1, childpv.moves,
                                       childpv.count * sizeof(struct move_t));
                                pv->count = childpv.count + 1;
                        }
                }
        }

        if (legal == 0) {
                pv->count = 0;
                if (check) {
                        return -MATE + board->ply;
                } else {
                        return 0;
                }
        }

        return alpha;
}


int count_nps(long nodes, long long time)
{
        if (time == 0)
                return 0;

        if (time > 20000)
                return nodes / (time / 1000);
        else
                return (nodes * 1000) / time;
}

void search(struct board_t *board, Search *search, PV *pv)
{

        int alpha, beta, score;

        alpha = -2 * MATE;
        beta = +2 * MATE;
        int current_depth;
        struct move_t bestmove = {0};

        for (current_depth = 1; current_depth <= search->depth;
             current_depth++) {
                score =
                    alphaBeta(alpha, beta, current_depth, board, search, pv);
                if (search->stop)
                        break;
                bestmove = pv->moves[0];
                printf("info depth %d score cp %d nodes %ld nps %d time %lld ",
                       current_depth, score, search->nodes,
                       count_nps(search->nodes,
                                 current_timestamp() - search->starttime),
                       current_timestamp() - search->starttime);
                printPV(pv);
                printf("\n");
        }
        printf("bestmove ");
        printMove(bestmove);
        printf("\n");
}

