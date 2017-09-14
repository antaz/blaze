#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "functions.h"

U64 pieceHash[13][120];
U64 turnHash;
U64 castleHash[16];

static U64 rand64() {
	return (rand() | rand() << 15 | rand() << 30 | rand() << 45 | rand() & 0xf << 60);
}

void initZobrist() {
	int i, j;
	
	for(i = 0; i < 13; i++) {
		for(j = 0; j < 120; j++) {
			pieceHash[i][j] = rand64();
		}
	}
	
	turnHash = rand64();

	for(i = 0; i < 16; i++) {
		castleHash[i] = rand64();
	}
}

U64 generateHash(Board *board) {
	int square;
	int piece;
	U64 zobrist = 0;
	
	for(square = 0; square < SQNUM; square++) {
		piece = board->pieces[square];
		if(piece != OFFBOARD && piece != EMPTY) {
			zobrist ^= pieceHash[piece][square];
		}
	}

	if(board->turn == WHITE) {
		zobrist ^= turnHash;
	}

	if(board->enPassant != NO_SQ) {
		zobrist ^= pieceHash[EMPTY][board->enPassant];
	}

	zobrist ^= castleHash[board->castling];

	return zobrist;
}
