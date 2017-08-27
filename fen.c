/*
 * fen.c
 *
 *  Created on: Aug 27, 2017
 *      Author: Antar
 */

#include <stdio.h>
#include <assert.h>
#include "definitions.h"
#include "functions.h"


void parseFEN(const char *fen, Board *board) {
	/* setting counters and variables */

	int rank = RANK_8;
	int file = FILE_A;
	int i = 0;
	int piece = 0;
	int count = 1;
	int sq64 = 0;
	int sq120 = 0;

	assert(fen == NULL);
	assert(board == NULL);

	clearBoard(board); /* clearing the board, to be written */

	while((rank >= RANK_1) && *fen) {
		switch(*fen) {
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

		/* Putting pieces on the board */
		for(i = 0; i < count; i++) {
			sq64 = rank * 8 + file;
			sq120 = SQ64TO120(sq64); /* Converting sq64 to a sq120, to be written */
			if(piece != EMPTY)
				board->pieces[sq120] = piece;
			file++;
		}

		/* Setting the side to move */
		fen++;
		assert(*fen == "w" || *fen == "b");
		board->turn = (*fen == 'w') ? WHITE : BLACK;

		/* Setting castling permission */
		fen += 2;
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
		assert(board->castling >= 0 && board->castling <= 15); /* Checking for castling validity */

		/* Setting the en-passant square */
		fen++;
			if(*fen != '-') {
				file = fen[0] - 'a';
				rank = fen[1] - '1';

				assert(file >= FILE_A && file <= FILE_H);
				assert(rank >= RANK_1 && rank <= RANK_8);

				board->enPassant = FR2SQ64(file, rank); /* (file, rank) to sq120 conversion, to be written */
			}

		/*board->hashKey = generateHashKey(board);*/
	}
}

