#include <stdio.h>
#include "definitions.h"
#include "functions.h"

void main() {
	int i;
	char *start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	char *fen = "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1";
	Board board[1];
	Search s[1];
	Move move;
	int score;
	initZobrist();
	board->table->table = NULL;
	MoveList list[1];
	PV pv;
	parseFEN(start_fen, board);
	initTTable(board->table);
	clearTTable(board->table);
	// generateCaptures(board, list);
	// printMoveList(list);
	/*
	char input[6];
	
	while(1) {
		printBoard(board);
		printf("Enter a move > ");
		fgets(input, 6, stdin);
		
		if(input[0] == 'q') {
			break;
		} else if(input[0] == 't') {
			takeMove(board);
		} else if (input[0] == 's') {
			//search(board, &pv);
			printf("static evaluation is: %d", evaluate(board));
		} else {
			if(parseMove(board, input, &move)) {
				makeMove(board, move);
			}
		}

		fflush(stdin);
	}
	*/
	//printf("\nWelcome to Trappist 1.0: Trappist on uci mode\n");
	uci_loop(board, s);

	//generateMoves(board, list);
	//qsort(list->moves, list->count, sizeof(Move), compareMoves);
	//printMoveList(list);
}
