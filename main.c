#include <stdio.h>
#include "definitions.h"
#include "functions.h"

void main() {
	int i;
	char *fen = "2rr2k1/5pp1/5b2/pb1BqP1Q/8/1P5P/P5P1/3R3K w - - 3 39";
	Board board[1];
	MoveList list[1];
	parseFEN(fen, board);
	search(board);
}
