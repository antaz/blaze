#include <stdio.h>
#include "definitions.h"
#include "functions.h"

#define MATE 99999

static void checkTime(Search *search) {
	if(search->timeset && current_timestamp() > search->stoptime)
		search->stop = 1;
}

static int qSearch(int alpha, int beta, Board *board, Search *search) {
	
	int movenum = 0;
	int legal = 0;
	int score;
	
	if((search->nodes & 0x3FF) == 0) {
		checkTime(search);
	}
	
	search->nodes++;
	
	MoveList list[1];
	score = evaluate(board);

	if(score >= beta) 
		return beta;
	if(score > alpha)
		alpha = score;
	
	generateCaptures(board, list);
	score = -2*MATE;
	
	for(movenum = 0; movenum < list->count; movenum++) {
		if(!makeMove(board, list->moves[movenum])) continue;
	
		legal++;
		score = -qSearch(-beta, -alpha, board, search);
		takeMove(board);
		
		if(search->stop) return 0;
		
		if(score > alpha) {
			if(score >= beta) return beta;
			alpha = score;
		}
	}
	return alpha; 
}

static int alphaBeta(int alpha, int beta, int depth, Board *board, Search *search, PV *pv) {
	
	int bestscore = -2*MATE;
	int score = -2*MATE;
	PV childpv;
	int movenum = 0;
	int legal = 0;
	int check;
	
	if((search->nodes & 0x3FF) == 0)
		checkTime(search);
	
	search->nodes++;
	
	if(depth == 0) {
		pv->count = 0;
		return qSearch(alpha, beta, board, search);
	}
	
	MoveList list[1];
	generateMoves(board, list);
	check = isAttacked(board, board->kingSquare[board->turn], board->turn ^ 1);

	for(movenum = 0; movenum < list->count; movenum++) {
		
		if(!makeMove(board, list->moves[movenum])) {
			continue;
		}
		
		legal++;
		score = -alphaBeta(-beta, -alpha, depth - 1, board, search, &childpv);
		takeMove(board);
		
		if(search->stop) {
			return 0;
		}

		if(score > bestscore) {
			if(score > alpha) {
				if(score >= beta) return beta;
				alpha = score;
				pv->moves[0] = list->moves[movenum];
				memcpy(pv->moves + 1, childpv.moves, childpv.count * sizeof(Move));
				pv->count = childpv.count + 1;
				
			}
			bestscore = score;
		}
	}
	
	if(legal == 0) {
		pv->count = 0;
		if(check) {
			return -MATE + board->ply;
		} else {
			return 0;
		}
	}
	
	return alpha;

}

void search(Board *board, Search *search, PV *pv) {

	int i;
	int alpha, beta, score;

	alpha = -2*MATE;
	beta = +2*MATE;
	int depth = search->depth;
	int currentDepth;
	Move bestmove;
	
	for(currentDepth = 1; currentDepth <= search->depth; currentDepth++) {
		score = alphaBeta(alpha, beta, currentDepth, board, search, pv);
		if(search->stop)
			break;
		bestmove = pv->moves[0];
		printf("info score cp %d depth %d nodes %d time %lld ", score, currentDepth, search->nodes, current_timestamp() - search->starttime);
		printPV(pv);
		printf("\n");
	}
	printf("bestmove ");
	printMove(bestmove);
	printf("\n");
}
