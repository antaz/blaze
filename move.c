#include <stdio.h>
#include <assert.h>
#include "definitions.h"
#include "functions.h"

void parseMove(Board *board, char *move, Move move) {
	int from, to;
	Move m;
	int moveNum = 0, Move = 0, promoted = EMPTY;
	MoveList list[1];

	if(move[1] > '8' || move[1] < '1') return 0;
	if(move[3] > '8' || move[3] < '1') return 0;
	if(move[0] > 'h' || move[0] < 'a') return 0;
	if(move[2] > 'h' || move[2] < 'a') return 0;
	
	from = FR2SQ120(move[0] - 'a', move[1] - '1');
	to = FR2SQ120(move[2] - 'a', move[3] - '1');
	
	generateMoves(board, list);
	for(moveNum = 0; moveNum < list->count; moveNum++) {
		m = list->moves[moveNum];
		if(m.from == from && m.to == to) {
			promoted = m.promoted;
			if(promoted != EMPTY) {
				if(isRook(promoted) && move[4] == 'r') {
					return m;
				} else if(isBishop(promoted) && move[4] == 'b') {
					return m;
				} else if(isQueen(promoted) && move[4] == 'q') {
					return m;
				} else if(isKnight(promoted) && move[4] == 'n') {
					return m;
				}
				continue;
			}
			return 1;
		}
	}
	
	return 0;
}

void moveSAN(Move move) {
	char *pieces = ".pnbrq";
	int promoted = move.promoted;
	assert(move.from != OFFBOARD);
	assert(move.to != OFFBOARD);
	if(promoted) {
		printf("%c%d%c%d%c", (FILE120(move.from) + 1) + 'a', RANK120(move.from) + 1, (FILE120(move.to) + 1) + 'a', RANK120(move.to) + 1,pieces[promoted % 6]);
	} else {
		printf("%c%d%c%d", (FILE120(move.from) + 1) + 'a', RANK120(move.from) + 1, (FILE120(move.to) + 1) + 'a', RANK120(move.to) + 1);
	}
}
