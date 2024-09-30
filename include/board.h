#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/* number of squares on the board */
#define SQ 64

/* number of piece types */
#define PC 7

/* maximum number of moves */
#define MM 256

/* no square, used for empty ep square */
#define NOSQ 8

/* colors and turns */
enum { WHITE = 0, BLACK = 1 };

/* pieces */
enum { NOPC, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

/* castling permissions */
enum {
	K = 1,	// White king side
	Q = 2,	// White queen side
	OK = 4, // Black king side
	OQ = 8	// Black queen side
};

/* non-recoverable information */
struct undo_t {
	uint8_t fifty; // fifty-move counter
	uint8_t ca;    // castling rights
	uint8_t ep;    // en-passent square
	uint8_t cap;   // captured piece (if any)
	uint64_t hash; // zobrist hash
};

/* main structure to hold the state of the board */
struct board_t {
	uint64_t bb[4];		// quad-bitboards
	uint8_t stm;		// side to move
	uint8_t ply;		// half-move count
	uint8_t hply;		// history half-move count
	uint8_t fifty;		// fifty-move counter
	uint8_t ca;		// castling rights
	uint8_t ep;		// en-passent square
	uint64_t hash;		// zobrist hash
	struct undo_t hist[MM]; // history of board states
};

void parse(struct board_t *board, const char *fen);
void flip(struct board_t *board);
void make(struct board_t *board, const uint16_t move);
void take(struct board_t *board, const uint16_t move);
uint64_t zobrist(struct board_t *board);

#endif /* BOARD_H */
