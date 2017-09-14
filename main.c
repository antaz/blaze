#include <stdio.h>
#include "definitions.h"
#include "functions.h"

void main() {
	int i;
	char *fen = "r3r1k1/p1q1np1p/4bQp1/3p2B1/2p5/2NB4/PPP2PPP/R3R1K1 w - - 0 18";
	Board board[1];
	initZobrist();
	MoveList list[1];
	parseFEN(fen, board);
	printBoardSAN(board);
	//search(board);
}
