#include <stdio.h>
#include "definitions.h"
#include "functions.h"

#define MATE 99999

static void check_time(Search *search) {
	if(search->timeset && current_timestamp() > search->stoptime)
		search->stop = 1;
}

static int quiscence(int alpha, int beta, int depth, Board *board) { return 0; }

static int alphaBeta(int alpha, int beta, int depth, Board *board, Search *search, PV *pv) {

	Move bestMove;
	int bestScore = -2*MATE;
	int score = -2*MATE;
	PV childpv;
	int moveNum = 0;
	int legal = 0;
	int inCheck;
	
	if((search->nodes & 0x3FF) == 0)
		check_time(search);
	
	search->nodes++;
	
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
		score = -alphaBeta(-beta, -alpha, depth - 1, board, search, &childpv);
		takeMove(board);
		
		if(search->stop) {
			return 0;
		}

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
		pv->count = 0;
		if(inCheck) {
			return -MATE + board->ply;
		} else {
			return 0;
		}
	}
	
	storeTable(board, bestMove, bestScore, HFEXACT, depth);
	return alpha;

}

void search(Board *board, Search *search, PV *pv) {

	int i;
	int alpha, beta, score;

	alpha = -2*MATE;
	beta = +2*MATE;
	int depth = search->depth;
	int currentDepth;
	
	for(currentDepth = 1; currentDepth <= depth; currentDepth++) {
		score = alphaBeta(alpha, beta, currentDepth, board, search, pv);
		
		if(search->stop)
			break;
		
		printf("info score cp %d depth %d nodes %d time %lld ", score, currentDepth, search->nodes, current_timestamp() - search->starttime);
		printPV(pv);
		printf("\n");
	}
	printf("bestmove ");
	moveSAN(pv->moves[0]);
	printf("\n");
}
