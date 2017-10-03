// Move generation functions

#include <stdio.h>
#include <assert.h>
#include "definitions.h"
#include "functions.h"

// direction of pieces
int pieceDirs[13][8] = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,1, 10, 0, 0, 0, 0 },
	{ -1, -10,1, 10, -9, -11, 11, 9 },
	{ -1, -10,1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,1, 10, 0, 0, 0, 0 },
	{ -1, -10,1, 10, -9, -11, 11, 9 },
	{ -1, -10,1, 10, -9, -11, 11, 9 }
};
// number of directions of each pieces (used with pieceDirs)
int numDirs[13] = { 0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8 };

// slider pieces and non-slider pieces used in the loop to generate moves
int sliderPieces[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
int nonSliderPieces[6] = {wN, wK, 0, bN, bK, 0};

// add move to list of moves (if pawn then addPawnMove())
void addMove(MoveList *list, char from, char to, char captured, char promoted, char enPassant, char pawnStart, char castle) {
	assert(onBoard(from));
	assert(onBoard(to));

	Move move;
	move.from = from;
	move.to = to;
	move.captured = captured;
	move.promoted = promoted;
	move.enPassant = enPassant;
	move.pawnStart = pawnStart;
	move.castle = castle;
	list->moves[list->count] = move;
	list->count++;
}
// adding a pawn move
void addPawnMove(Board *board, MoveList *list, char from, char to, char captured, char enPassant, char pawnStart, char castle) {
	assert(onBoard(from));
	assert(onBoard(to));
	
	// checking for promotion
	if(board->pieces[from] == wP && RANK120(from) == RANK_7) {
		addMove(list, from, to, captured, wQ, enPassant, pawnStart, castle);
		addMove(list, from, to, captured, wR, enPassant, pawnStart, castle);
		addMove(list, from, to, captured, wB, enPassant, pawnStart, castle);
		addMove(list, from, to, captured, wN, enPassant, pawnStart, castle);	
	} else if(board->pieces[from] == bP && RANK120(from) == RANK_2) {
		addMove(list, from, to, captured, bQ, enPassant, pawnStart, castle);
		addMove(list, from, to, captured, bR, enPassant, pawnStart, castle);
		addMove(list, from, to, captured, bB, enPassant, pawnStart, castle);
		addMove(list, from, to, captured, bN, enPassant, pawnStart, castle);
	} else {
		addMove(list, from, to, captured, EMPTY, enPassant, pawnStart, castle);
	}
}
// TODO later, give different scoring to quiet and capture moves

void generateMoves(Board *board, MoveList *list) {
	
	// setting some variables
	list->count = 0;
	int piece = EMPTY;
	int turn = board->turn;
	int pieceCount = 0;
	int square = 0;
	int pieceIndex;
	int index;
	int dir;
	int targetSquare;
	
	// generating pawn moves
	if(turn == WHITE) {
		// Adding white pawn moves
		for(pieceCount = 0; pieceCount < board->pieceCount[wP]; ++pieceCount) {
			square = board->pieceList[wP][pieceCount];
			if(board->pieces[square + 10] == EMPTY) {
				addPawnMove(board, list, square, square + 10, EMPTY, 0, 0, 0);
				if(RANK120(square) == RANK_2 && board->pieces[square + 20] == EMPTY) {
					addMove(list, square, square + 20, EMPTY, EMPTY, 0, 1, 0);
				}
			}
			// Adding white pawn captures
			if(board->pieces[square + 9] != OFFBOARD && board->pieces[square + 9] != EMPTY && pieceColor(board->pieces[square + 9]) == BLACK) {
				addPawnMove(board, list, square, square + 9, board->pieces[square + 9], 0, 0, 0);
			}
			if(board->pieces[square + 11] != OFFBOARD && board->pieces[square + 11] != EMPTY && pieceColor(board->pieces[square + 11]) == BLACK) {
				addPawnMove(board, list, square, square + 11, board->pieces[square + 11], 0, 0, 0);
			}
			// Adding white pawn enpassant captures
			if(board->enPassant != NO_SQ) {
				if(square + 9 == board->enPassant) {
					addMove(list, square, square + 9, EMPTY, EMPTY, 1, 0, 0);
				}
				if(square + 11 == board->enPassant) {
					addMove(list, square, square + 11, EMPTY, EMPTY, 1, 0, 0);
				}
			}
		}
		// adding white's kingside and queenside castling moves
		if(board->castling & WKCA) {
			if(board->pieces[F1] == EMPTY && board->pieces[G1] == EMPTY) {
				if(!isAttacked(board, E1, BLACK) && !isAttacked(board, F1, BLACK)) {
					addMove(list, E1, G1, EMPTY, EMPTY, 0, 0, 1);
				}
			}
		}		

		if(board->castling & WQCA) {
			if(board->pieces[D1] == EMPTY && board->pieces[C1] == EMPTY && board->pieces[B1] == EMPTY) {
				if(!isAttacked(board, E1, BLACK) && !isAttacked(board, D1, BLACK)) {
					addMove(list, E1, C1, EMPTY, EMPTY, 0, 0, 1);
				}
			}
		}
	} else {
		// adding black pawn moves
		for(pieceCount = 0; pieceCount < board->pieceCount[bP]; ++pieceCount) {
			square = board->pieceList[bP][pieceCount];
			if(board->pieces[square - 10] == EMPTY) {
				addPawnMove(board, list, square, square - 10, EMPTY, 0, 0, 0);
				if(RANK120(square) == RANK_7 && board->pieces[square - 20] == EMPTY) {
					addMove(list, square, square - 20, EMPTY, EMPTY, 0, 1, 0);
				}
			}
			// adding black pawn captures
			if(board->pieces[square - 9] != OFFBOARD && board->pieces[square - 9] != EMPTY && pieceColor(board->pieces[square - 9]) == WHITE) {
				addPawnMove(board, list, square, square - 9, board->pieces[square - 9], 0, 0, 0);
			}
			if(board->pieces[square - 11] != OFFBOARD && board->pieces[square -11] != EMPTY && pieceColor(board->pieces[square - 11]) == WHITE) {
				addPawnMove(board, list, square, square - 11, board->pieces[square - 11], 0, 0, 0);
			}
			// adding black pawn enpassant captures
			if(board->enPassant != NO_SQ) {
				if(square - 9 == board->enPassant) {
					addMove(list, square, square - 9, EMPTY, EMPTY, 1, 0, 0);
				}
				if(square - 11 == board->enPassant) {
					addMove(list, square, square - 11, EMPTY, EMPTY, 1, 0, 0);
				}
			}
		}
		// adding black's kingside and queenside castling moves
		if(board->castling & BKCA) {
			if(board->pieces[F8] == EMPTY && board->pieces[G8] == EMPTY) {
				if(!isAttacked(board, E8, WHITE) && !isAttacked(board, F8, WHITE)) {
					addMove(list, E8, G8, EMPTY, EMPTY, 0, 0, 1);
				}
			}
		}		

		if(board->castling & BQCA) {
			if(board->pieces[D8] == EMPTY && board->pieces[C8] == EMPTY && board->pieces[B8] == EMPTY) {
				if(!isAttacked(board, E8, WHITE) && !isAttacked(board, D8, WHITE)) {
					addMove(list, E8, C8, EMPTY, EMPTY, 0, 0, 1);
				}
			}
		}
	}
	// generating slider pieces moves
	pieceIndex = turn*4;
	piece = sliderPieces[pieceIndex++];
	while(piece != 0) {
		for(pieceCount = 0; pieceCount < board->pieceCount[piece]; pieceCount++) {
			square = board->pieceList[piece][pieceCount];
			for(index = 0; index < numDirs[piece]; index++) {
				dir = pieceDirs[piece][index];
				targetSquare = square + dir;
				
				while(board->pieces[targetSquare] != OFFBOARD) {
					if(board->pieces[targetSquare] != EMPTY) {
						if(pieceColor(board->pieces[targetSquare]) == turn ^ 1) {
							addMove(list, square, targetSquare, board->pieces[targetSquare], EMPTY, 0, 0, 0);
						}
						break;
					}
					addMove(list, square, targetSquare, EMPTY, EMPTY, 0, 0, 0);
					targetSquare += dir;
				}
			}	
		}
		piece = sliderPieces[pieceIndex++];
	}
	// generating non-slider pieces moves
	pieceIndex = turn*3;
	piece = nonSliderPieces[pieceIndex++];
	while(piece != 0) {
		for(pieceCount = 0; pieceCount < board->pieceCount[piece]; pieceCount++) {
			square = board->pieceList[piece][pieceCount];
			for(index = 0; index < numDirs[piece]; index++) {
				dir = pieceDirs[piece][index];
				targetSquare = square + dir;
				if(board->pieces[targetSquare] == OFFBOARD) {
					continue;
				}	
				if(board->pieces[targetSquare] != EMPTY) {
					if(pieceColor(board->pieces[targetSquare]) == turn ^ 1) {
						addMove(list, square, targetSquare, board->pieces[targetSquare], EMPTY, 0, 0, 0);
					}
					continue;
				}
				addMove(list, square, targetSquare, EMPTY, EMPTY, 0, 0, 0);
			}	
		}
		piece = nonSliderPieces[pieceIndex++];
	}
}

void generateCaptures(Board *board, MoveList *list) {
	

	// setting some variables
	list->count = 0;
	int piece = EMPTY;
	int turn = board->turn;
	int pieceCount = 0;
	int square = 0;
	int pieceIndex;
	int index;
	int dir;
	int targetSquare;
	
	// generating pawn moves
	if(turn == WHITE) {
		// Adding white pawn moves
		for(pieceCount = 0; pieceCount < board->pieceCount[wP]; ++pieceCount) {
			square = board->pieceList[wP][pieceCount];
			// Adding white pawn captures
			if(board->pieces[square + 9] != OFFBOARD && board->pieces[square + 9] != EMPTY && pieceColor(board->pieces[square + 9]) == BLACK) {
				addPawnMove(board, list, square, square + 9, board->pieces[square + 9], 0, 0, 0);
			}
			if(board->pieces[square + 11] != OFFBOARD && board->pieces[square + 11] != EMPTY && pieceColor(board->pieces[square + 11]) == BLACK) {
				addPawnMove(board, list, square, square + 11, board->pieces[square + 11], 0, 0, 0);
			}
			// Adding white pawn enpassant captures
			if(board->enPassant != NO_SQ) {
				if(square + 9 == board->enPassant) {
					addMove(list, square, square + 9, EMPTY, EMPTY, 1, 0, 0);
				}
				if(square + 11 == board->enPassant) {
					addMove(list, square, square + 11, EMPTY, EMPTY, 1, 0, 0);
				}
			}
		}
	} else {
		// adding black pawn moves
		for(pieceCount = 0; pieceCount < board->pieceCount[bP]; ++pieceCount) {
			square = board->pieceList[bP][pieceCount];
			// adding black pawn captures
			if(board->pieces[square - 9] != OFFBOARD && board->pieces[square - 9] != EMPTY && pieceColor(board->pieces[square - 9]) == WHITE) {
				addPawnMove(board, list, square, square - 9, board->pieces[square - 9], 0, 0, 0);
			}
			if(board->pieces[square - 11] != OFFBOARD && board->pieces[square -11] != EMPTY && pieceColor(board->pieces[square - 11]) == WHITE) {
				addPawnMove(board, list, square, square - 11, board->pieces[square - 11], 0, 0, 0);
			}
			// adding black pawn enpassant captures
			if(board->enPassant != NO_SQ) {
				if(square - 9 == board->enPassant) {
					addMove(list, square, square - 9, EMPTY, EMPTY, 1, 0, 0);
				}
				if(square - 11 == board->enPassant) {
					addMove(list, square, square - 11, EMPTY, EMPTY, 1, 0, 0);
				}
			}
		}
	}
	// generating slider pieces moves
	pieceIndex = turn*4;
	piece = sliderPieces[pieceIndex++];
	while(piece != 0) {
		for(pieceCount = 0; pieceCount < board->pieceCount[piece]; pieceCount++) {
			square = board->pieceList[piece][pieceCount];
			for(index = 0; index < numDirs[piece]; index++) {
				dir = pieceDirs[piece][index];
				targetSquare = square + dir;
				
				while(board->pieces[targetSquare] != OFFBOARD) {
					if(board->pieces[targetSquare] != EMPTY) {
						if(pieceColor(board->pieces[targetSquare]) == turn ^ 1) {
							addMove(list, square, targetSquare, board->pieces[targetSquare], EMPTY, 0, 0, 0);
						}
						break;
					}
					targetSquare += dir;
				}
			}	
		}
		piece = sliderPieces[pieceIndex++];
	}
	// generating non-slider pieces moves
	pieceIndex = turn*3;
	piece = nonSliderPieces[pieceIndex++];
	while(piece != 0) {
		for(pieceCount = 0; pieceCount < board->pieceCount[piece]; pieceCount++) {
			square = board->pieceList[piece][pieceCount];
			for(index = 0; index < numDirs[piece]; index++) {
				dir = pieceDirs[piece][index];
				targetSquare = square + dir;
				if(board->pieces[targetSquare] == OFFBOARD) {
					continue;
				}	
				if(board->pieces[targetSquare] != EMPTY) {
					if(pieceColor(board->pieces[targetSquare]) == turn ^ 1) {
						addMove(list, square, targetSquare, board->pieces[targetSquare], EMPTY, 0, 0, 0);
					}
					continue;
				}
			}	
		}
		piece = nonSliderPieces[pieceIndex++];
	}
}
