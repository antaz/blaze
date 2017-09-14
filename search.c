#include <stdio.h>
#include "definitions.h"
#include "functions.h"

#define INFINITE 99999
static int quiscence(int alpha, int beta, int depth, Board *board) { return 0; }

static int alphaBeta(int alpha, int beta, int depth, Board *board, PvLine *pLine) {

	Move bestMove;
	int bestScore = -INFINITE;
	int score = -INFINITE;
	PvLine line;
	int moveNum = 0;
	int legal = 0;
	int inCheck;
	
	if(depth == 0) {
		pLine->movesCount = 0;
		return evaluate(board);
	}
	
	MoveList list[1];
	generateMoves(board, list);
	
	inCheck = isAttacked(board->kingSquare[board->turn], board, board->turn ^ 1);
	
	for(moveNum = 0; moveNum < list->count; moveNum++) {
		if(!makeMove(board, list->moves[moveNum])) {
			continue;
		}
		legal++;
		score = -alphaBeta(-beta, -alpha, depth - 1, board, &line);
		takeMove(board);
		
		if(score >= beta) return beta;
		if(score > alpha) {
			alpha = score;
			pLine->moves[0] = list->moves[moveNum];
			memcpy(pLine->moves + 1, line.moves, line.movesCount * sizeof(Move));
			pLine->movesCount = line.movesCount + 1;
		}
	}
	
	if(legal == 0) {
		if(inCheck) {
			return -INFINITE + board->ply;
		} else {
			return 0;
		}
	}
	
	return alpha;

}

void search(Board *board) {

	PvLine line[1];
	int i;
	int depth = 4;
	int bestScore = -INFINITE;
	int currentDepth = 0;
	for(currentDepth = 0; currentDepth <= depth; currentDepth++) {
		bestScore = alphaBeta(-INFINITE, INFINITE, currentDepth, board, line);
	}
	printf("\nMoves in PV table: \n");
	for(i = 0; i < line->movesCount; i++) {
		moveSAN(line->moves[i]);
		printf(" ");
	}
	printf("\nScore is: %d\n", bestScore);

}
