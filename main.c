#include <stdio.h>
#include "definitions.h"
#include "functions.h"

void main() {
	int i;
	char *fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Board board[1];
	Move move;
	initZobrist();
	board->table->table = NULL;
	MoveList list[1];
	parseFEN(fen, board);
	initTTable(board->table);
	
	char input[6];
	
	while(1) {
		printBoardSAN(board);
		printf("Enter a move > ");
		fgets(input, 6, stdin);
		
		if(input[0] == 'q') {
			break;
		} else if(input[0] == 't') {
			takeMove(board);
		} else {
			parseMove(board, input);
			moveSAN(move);
		}

		fflush(stdin);
	}
	//search(board);
}
