/*
 * board.c
 *
 *  Created on: Aug 27, 2017
 *      Author: Antar
 */

#include <stdio.h>
#include <string.h>
#include "definitions.h"
#include "functions.h"

void clearBoard(Board *board) {
	int i;
	memset(board->pieces, OFFBOARD, SQNUM-1);
	for(i = 0; i < 64; i++) {
		board->pieces[SQ64TO120(i)] == EMPTY;
		board->turn = 0;
		board->ply = 0;
		board->castling = 0;
		board->enPassant = NO_SQ;
		board->kingSq[WHITE] = board->kingSq[BLACK] = NO_SQ;
		board->fiftyMove = 0;
	}
}
