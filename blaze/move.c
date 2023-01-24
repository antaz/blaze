#include <stdio.h>
#include "definitions.h"
#include "functions.h"

int parseMove(Board *board, char *move, Move *m) {
	int from, to;
	int moveNum = 0, promoted = EMPTY;
	MoveList list[1];

	if(move[1] > '8' || move[1] < '1') return 0;
	if(move[3] > '8' || move[3] < '1') return 0;
	if(move[0] > 'h' || move[0] < 'a') return 0;
	if(move[2] > 'h' || move[2] < 'a') return 0;
	
	from = FR2SQ120(move[0] - 'a', move[1] - '1');
	to = FR2SQ120(move[2] - 'a', move[3] - '1');
	
	generateMoves(board, list);
	for(moveNum = 0; moveNum < list->count; moveNum++) {
		m->from = list->moves[moveNum].from;
		m->to = list->moves[moveNum].to;
		m->captured = list->moves[moveNum].captured;	
		m->promoted = list->moves[moveNum].promoted;
		m->enPassant = list->moves[moveNum].enPassant;
		m->pawnStart = list->moves[moveNum].pawnStart;
		m->castle = list->moves[moveNum].castle;
		if(m->from == from && m->to == to) {
			promoted = m->promoted;
			if(promoted != EMPTY) {
				if(isRook(promoted) && move[4] == 'r') {
					return 1;
				} else if(isBishop(promoted) && move[4] == 'b') {
					return 1;
				} else if(isQueen(promoted) && move[4] == 'q') {
					return 1;
				} else if(isKnight(promoted) && move[4] == 'n') {
					return 1;
				}
				continue;
			}
			return 1;
		}
	}
	return 0;
}

void printMove(Move move) {
	char *pieces = ".pnbrq";
	int promoted = move.promoted;
	ASS(move.from != OFFBOARD);
	ASS(move.to != OFFBOARD);
	if (move.from == 0 && move.to == 0) {
		printf("0000");
		return;
	}
	if(promoted) {
		printf("%c%d%c%d%c", (FILE120(move.from)) + 'a', RANK120(move.from) + 1, (FILE120(move.to)) + 'a', RANK120(move.to) + 1,pieces[promoted % 6]);
	} else {
		printf("%c%d%c%d", (FILE120(move.from)) + 'a', RANK120(move.from) + 1, (FILE120(move.to)) + 'a', RANK120(move.to) + 1);
	}
}
