#include <stdio.h>
#include "definitions.h"
#include "functions.h"

#define MATE 99999
static int quiscence(int alpha, int beta, int depth, Board *board) { return 0; }

static int alphaBeta(int alpha, int beta, int depth, Board *board, PV *pv) {

	Move bestMove;
	int bestScore = -2*MATE;
	int score = -2*MATE;
	PV childpv;
	int moveNum = 0;
	int legal = 0;
	int inCheck;
	
	if(depth == 0) {
		pv->count = 0;
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
		score = -alphaBeta(-beta, -alpha, depth - 1, board, &childpv);
		takeMove(board);
		
		if(score > bestScore) {
			if(score > alpha) {
				if(score >= beta) return beta;
				alpha = score;
				pv->moves[0] = list->moves[moveNum];
				memcpy(pv->moves + 1, childpv.moves, childpv.count * sizeof(Move));
				pv->count = childpv.count + 1;
				
			}
			bestScore = score;
			bestMove = list->moves[moveNum];
		}
	}
	
	if(legal == 0) {
		if(inCheck) {
			pv->count = 0;
			return -MATE + board->ply;
		} else {
			pv->count = 0;
			return 0;
		}
	}
	
	storeTable(board, bestMove, bestScore, HFEXACT, depth);
	return alpha;

}

void search(Board *board, PV *pv) {

	PV *line;
	int i;
	int depth = 4;
	int alpha, beta, score;
	
	alpha = -2*MATE;
	beta = +2*MATE;
	int currentDepth = 0;

	for(currentDepth = 0; currentDepth <= depth; currentDepth++) {
		score = alphaBeta(alpha, beta, currentDepth, board, pv);
	}
}
