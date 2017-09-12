#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdbool.h>

// A couple of constants for our chess engine

#define NAME "Trappist"
#define SQNUM 120 // Number of squares on the board
#define U64 unsigned long long
#define MAX 256

//pieces
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK }; 

// files
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };

// ranks
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };
// colors
enum { WHITE, BLACK, BOTH };

// square numbers
enum {
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8,
	NO_SQ, OFFBOARD
};

// Castling permission constants
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

// The main board structure for our chess engine
typedef struct {
	int pieces[SQNUM];
	int pieceCount[13]; // The count of pieces on the board
	int pieceList[13][10]; // The squares of where pieces are
	int kingSquare[2];
	int turn, enPassant, fiftyMove;
	int ply, castling;

	U64 hashKey;

} Board;

typedef struct {
	char from; // move from
	char to; // move destination
	char captured; // captured piece
	char promoted; // promoted piece
	bool enPassant; // en-passant flag 
	bool pawnStart; // pawn start flag
	bool castle; // castling flag
} Move;

typedef struct {
	Move moves[MAX];
	int count;
} MoveList;
#endif
