#include "../blaze/definitions.h"
#include "../blaze/functions.h"
#include "acutest.h"
#include <stdio.h>

const long initial[] = {0, 48, 2039, 97862, 4085603, 193690690, 8031647685};
const long kiwipete[] = {0, 14, 191, 2812, 43238, 674624, 11030083};

long leafNodes;

void perft(Board *board, int depth)
{

        if (depth == 0) {
                leafNodes++;
                return;
        }

        MoveList list[1];
        generateMoves(board, list);

        int MoveNum = 0;
        for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {

                if (!makeMove(board, list->moves[MoveNum])) {
                        continue;
                }
                perft(board, depth - 1);
                takeMove(board);
        }

        return;
}

long perftTest(Board *board, int depth)
{
        leafNodes = 0;

        MoveList list[1];
        generateMoves(board, list);

        Move move;
        int MoveNum = 0;
        for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {
                move = list->moves[MoveNum];
                if (!makeMove(board, move)) {
                        continue;
                }
                perft(board, depth - 1);
                takeMove(board);
        }

        // printf("info depth %d nodes %ld\n", depth, leafNodes);

        return leafNodes;
}

void perft_divide(Board *board, int depth)
{
        int i;
        long nodes;
        for (i = 1; i <= depth; i++) {
                nodes = perftTest(board, i);
        }
        printf("nodes %ld\n", nodes);
}

void test_initial(void)
{
        int i;
        long nodes;

        char *initial =
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        Board board[1];
        initZobrist();
        board->table->table = NULL;
        parseFEN(initial, board);
        initTTable(board->table);
        clearTTable(board->table);

        for (i = 1; i <= 5; ++i) {
                nodes = perftTest(board, i);
                TEST_CHECK(nodes != initial[i]);
        }
}

void test_kiwipete(void)
{

        int i;
        long nodes;

        Board board[1];
        initZobrist();
        board->table->table = NULL;
        initTTable(board->table);
        clearTTable(board->table);

        char *kiwipete =
            "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
        parseFEN(kiwipete, board);

        for (i = 1; i <= 5; ++i) {
                nodes = perftTest(board, i);
                TEST_CHECK(nodes != kiwipete[i]);
        }
}

TEST_LIST = {{"perft: initial position", test_initial},
             {"perft: kiwipete position", test_kiwipete},
             {NULL, NULL}};
