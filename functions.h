/*
 * functions.h
 *
 *  Created on: Aug 27, 2017
 *      Author: Antar
 */


#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Macro functions
#define FR2SQ64(f, r) (((r) << 3) | (f))
#define FR2SQ120(f,r) (((r)+2)*10 + (f) + 1)
#define RANK(sq) ((sq) >> 3)
#define FILE(sq) ((sq) & 7)
#define RANK120(sq) ((sq) / 10 - 2) 
#define FILE120(sq) ((sq) % 10 - 2)
#define SQ64TO120(sq) ((((sq) >> 3)+2)*10 + ((sq) & 7) + 1)
#define isPawn(piece) (piece == 1 || piece == 7)
#define isKnight(piece) (piece == 2 || piece == 8)
#define isBishop(piece) (piece == 3 || piece == 9)
#define isRook(piece) (piece == 4 || piece == 10)
#define isQueen(piece) (piece == 5 || piece == 11)
#define isKing(piece) (piece == 6 || piece == 12)
#define pieceColor(piece) ((piece > 0 && piece <= 6) ? 0 : 1)
#define isSlidePiece(piece) (isBishop(piece) | isRook(piece) | isQueen(piece))

// board.c
extern void clearBoard(Board *board);
extern void printBoard(Board *board);
extern void parseFEN(char *fen, Board *board);
extern void updatePieceList(Board *board);

// move.c
extern void moveSAN(Move move);

// movegen.c
extern void generateMoves(Board *board, MoveList *list);

// makemove.c
extern int makeMove(Board *board, Move move);
extern void takeMove(Board *board);

// evaluation.c
extern int evaluate(Board *board);

// search.c

extern void search(Board *board);

// hash.c

extern void initZobrist();
extern void setBoardHash(Board *board);
#endif
