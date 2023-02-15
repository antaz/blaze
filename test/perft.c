#include "../blaze/definitions.h"
#include "../blaze/functions.h"
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

int main()
{
        return 0;
}
