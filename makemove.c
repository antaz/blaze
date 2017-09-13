#include <stdio.h>
#include "definitions.h"
#include "functions.h"

const int castlePerm[120] = {
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void clearPiece(Board *board, int square) {
	int piece = board->pieces[square];
	int index = 0;
	int pieceNum = -1;
	
	// removing the piece from the board->pieces
	board->pieces[square] = EMPTY;
	
	// removing the piece from the board->pieceList
	for(index = 0; index < board->pieceCount[piece]; index++) {
		if(board->pieceList[piece][index] == square) {
			pieceNum = index;
			break;
		}
	}
	
	// decrementing the pieceCount and the pieceList
	board->pieceCount[piece]--;
	board->pieceList[piece][pieceNum] = board->pieceList[piece][board->pieceCount[piece]];
}

static void addPiece(Board *board, int square, int piece) {
	
	// adding the piece to the board->pieces
	board->pieces[square] = piece;
	// adding the piece to the board->pieceList
	board->pieceList[piece][board->pieceCount[piece]++] = square;
}

static void movePiece(Board *board, int from, int to) {
	int index = 0;
	int piece = board->pieces[from];
	
        // remove piece from and it to to
	board->pieces[from] = EMPTY;
	board->pieces[to] = piece;
	
	// move the piece from -> to in the board->pieceList
	for(index = 0; index < board->pieceCount[piece]; index++) {
		if(board->pieceList[piece][index] == from) {
			board->pieceList[piece][index] = to;
			break;
		}
	}
}

int makeMove(Board *board, Move move) {
	
	// setting the move variables
	int from = move.from;
	int to = move.to;
	int captured = move.captured;
	int promoted = move.promoted;
	int turn = board->turn;
	
	// if enpassant move remove the pawn captured
	if(move.enPassant) {
		if(turn == WHITE) {
			clearPiece(board, to - 10);
		} else {
			clearPiece(board, to + 10);
		}
	// castle moves
	} else if(move.castle) {
		switch(to) {
			case C1: movePiece(board, A1, D1); break;
			case C8: movePiece(board, A8, D8); break;
			case G1: movePiece(board, H1, F1); break;
			case G8: movePiece(board, H8, F8); break;
			default: break;
		}
	}
	
	// updating the history array with history information
	board->history[board->hisPly].move = move;
	board->history[board->hisPly].fiftyMove = board->fiftyMove;
	board->history[board->hisPly].enPassant = board->enPassant;
	board->history[board->hisPly].castling = board->castling;

	// changing the castling permission after a castle is made
	board->castling &= castlePerm[from];
	board->castling &= castlePerm[to];
	
	// resetting the enpassant square
	board->enPassant = NO_SQ;
	
	// clearing the captured piece and resetting the fifity move rule
	if(captured != EMPTY) {
		clearPiece(board, to);
		board->fiftyMove = 0;
	}
	
	// incrementing the game ply and current search ply
	board->hisPly++;
	board->ply++;
	
	// if pawn start set enpassant square
	if(isPawn(board->pieces[from])) {
		if(move.pawnStart) {
			if(turn == WHITE) {
				board->enPassant = from + 10;
			} else {
				board->enPassant = from - 10;
			}
		}
	}
	
	// move the piece
	movePiece(board, from, to);
	
	// if promotion clear the pawn and add the piece promoted
	if(promoted != EMPTY) {
		clearPiece(board, to);
		addPiece(board, to, promoted);
	}

	// set the kingSquare
	if(isKing(board->pieces[to])) {
		board->kingSquare[turn] = to;
	}
	
	// switch turn
	board->turn ^= 1;

	// if in check take back the move
	if(isAttacked(board->kingSquare[turn], board, board->turn)){
		takeMove(board);
		return 0;
	}
	
	return 1;
}

void takeMove(Board *board) {
	
	// decrementing the game play and current search ply
	board->hisPly--;
	board->ply--;
	
	// setting the history move variables
	Move move = board->history[board->hisPly].move;
	int from = move.from;
	int to = move.to;
	int captured = move.captured;
	int promoted = move.promoted;

	// updating the board with history information
	board->castling = board->history[board->hisPly].castling;
	board->fiftyMove = board->history[board->hisPly].fiftyMove;
	board->enPassant = board->history[board->hisPly].enPassant;

	// switch the turn
	board->turn ^= 1;

	// if enpassant add the pawn back
	if(move.enPassant) {
		if(board->turn == WHITE) {
			addPiece(board, to - 10, bP);
		} else {
			addPiece(board, to + 10, wP);
		}
	} 
	// set castling moves
	else if(move.castle) {
		switch(to) {
			case C1: movePiece(board, D1, A1); break;
			case C8: movePiece(board, D8, A8); break;
			case G1: movePiece(board, F1, H1); break;
			case G8: movePiece(board, F8, H8); break;
			default: ; break;
		}
	}
	
	// move the piece
	movePiece(board, to, from);

	// set the kingSquare
	if(isKing(board->pieces[from])) {
		board->kingSquare[board->turn] = from;
	}
	
	// adding back the captured piece
	if(captured != EMPTY) {
		addPiece(board, to, captured);
	}

	// clearing the promoted piece and adding the pawn promoted to the board
	if(promoted != EMPTY) {
		clearPiece(board, from);
		addPiece(board, from, (pieceColor(promoted) == WHITE ? wP : bP));
	}

}
