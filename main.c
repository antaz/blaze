#include <stdio.h>
#include "definitions.h"
#include "functions.h"

void main() {
	int i;
	char *start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	char *fen = "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -";
	Board board[1];
	Move move;
	int score;
	initZobrist();
	board->table->table = NULL;
	MoveList list[1];
	//PV pv;
	parseFEN(fen, board);
	initTTable(board->table);
	/*
	char input[6];
	
	while(1) {
		printBoardSAN(board);
		printf("Enter a move > ");
		fgets(input, 6, stdin);
		
		if(input[0] == 'q') {
			break;
		} else if(input[0] == 't') {
			takeMove(board);
		} else if (input[0] == 's') {
			search(board, &pv);
			//printf("static evaluation is: %d", evaluate(board));
		} else {
			if(parseMove(board, input, &move)) {
				makeMove(board, move);
			}
		}

		fflush(stdin);
	}*/
	uci_loop(board);
}
