#include <stdio.h>
#include "definitions.h"
#include "functions.h"

void main() {
	char *fen = "r1bqkbnr/1ppp1ppp/p1n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4";
	printf("Under Construction!! :)");
	Board board[1];
	clearBoard(board);
	parseFEN(fen, board);	
	printBoardSAN(board);

	Move move[1];
	move->from = A2;
	move->to = H7;
	move->captured = wP;
	move->promoted = wR;

	moveSAN(move);
}

// sq120 -> san e.g 21 -> a1

// 21 is in Rank 3 file 2 -> 3 - 2 = 1 rank 1 is 1, 2 - 1 = 1 file 1 is a
