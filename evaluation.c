#include <stdio.h>
#include "definitions.h"
#include "functions.h"


int evaluate(Board *board) {
	int score = board->material[WHITE] - board->material[BLACK];
	if(board->turn == WHITE) {
		return score;
	} else {
		return -score;
	}
}
