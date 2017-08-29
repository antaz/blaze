/*
 * board.c
 *
 *  Created on: Aug 27, 2017
 *      Author: Antar
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "definitions.h"
#include "functions.h"

// parsing an FEN string 
void parseFEN(char *fen, Board *board) {
	int rank = RANK_8;
	int file = FILE_A;
	int piece = 0;
	int count = 0;
	int i = 0;
	int sq64 = 0;
	int sq120 = 0;

	assert(fen != NULL);
	assert(board != NULL);

	clearBoard(board); // clearing and resetting the board

	while((rank >= RANK_1) && *fen) {
		count = 1;
		switch(*fen) {
			// in case of a piece 
			case 'p': piece = bP; break;
			case 'r': piece = bR; break;
			case 'n': piece = bN; break;
			case 'b': piece = bB; break;
			case 'k': piece = bK; break;
			case 'q': piece = bQ; break;
			case 'P': piece = wP; break;
			case 'R': piece = wR; break;
			case 'N': piece = wN; break;
			case 'B': piece = wB; break;
			case 'K': piece = wK; break;
			case 'Q': piece = wQ; break;

			// in case of a number
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
				piece = EMPTY;
				count = *fen - '0';
				break;

			case '/':
			case ' ':
				rank--;
				file = FILE_A;
				fen++;
				continue;

			default:
				printf("Invalid FEN string!\n");
		}
		
		// setting pieces on squares
		for(i = 0; i < count; i++) {
			sq64 = rank * 8 + file;
			sq120 = SQ64TO120(sq64);
			if(piece != EMPTY)
				board->pieces[sq120] = piece;
			file++;
		}
		fen++;
	}

	assert(*fen == 'w' || *fen == 'b');
	
	// setting the side to move
	board->turn = (*fen == 'w') ? WHITE : BLACK;
	fen += 2;

	// setting castling permissions
	for(i = 0; i < 4; i++) {
		if(*fen == ' ')
			break;
		switch(*fen) {
			case 'K': board->castling |= WKCA; break;
			case 'Q': board->castling |= WQCA; break;
			case 'k': board->castling |= BKCA; break;
			case 'q': board->castling |= BQCA; break;		
		}
		fen++;
	}
	fen++;

	assert(board->castling >= 0 && board->castling <= 15);

	// setting the en-passant square
	if(*fen != '-') {
		file = fen[0] - 'a';
		rank = fen[1] - '1';

		assert(file >= FILE_A && file <= FILE_H);
		assert(rank >= RANK_1 && rank <= RANK_8);

		board->enPassant = FR2SQ120(file, rank);
	}

}

// clearing the board function
void clearBoard(Board *board) {
	int i;
	for(i = 0; i < SQNUM; i++)
		board->pieces[i] = OFFBOARD;
	
	for(i = 0; i < 64; i++) {
		board->pieces[SQ64TO120(i)] = EMPTY;
	}
	board->turn = 0;
	board->ply = 0;
	board->castling = 0;
	board->enPassant = NO_SQ;
	board->kingSq[WHITE] = board->kingSq[BLACK] = NO_SQ;
	board->fiftyMove = 0;
}

// printing the board in std algebraic notation
void printBoardSAN(Board *board) {
	int file, rank, sq, piece;
	char pieceChar[] = ".PNBRQKpnbrqk";

	printf("\n\n");
	for(rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d ", rank+1);
		for(file = FILE_A; file <= FILE_H; file++) {
			sq = FR2SQ120(file, rank);
			piece = board->pieces[sq];
			printf("%3c", pieceChar[piece]);
		}
		printf("\n");
	}
	printf("\n  ");
		for(file = FILE_A; file <= FILE_H; file++)
			printf("%3c", 'a' + file);
	printf("\n");
}
