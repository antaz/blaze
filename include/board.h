#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/* number of squares on the board */
#define SQ 64

/* number of piece types */
#define PC 6

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
    WK = 0x02, // White king side
    WQ = 0x01, // White queen side
    BK = 0x20, // Black king side
    BQ = 0x10  // Black queen side
};

/* non-recoverable information */
struct undo_t {
    uint8_t ca;  // castling rights
    uint8_t ep;  // en-passent square
    uint8_t cap; // captured piece (if any)
};

/* main structure to hold the state of the board */
struct board_t {
    uint64_t bb[4];         // quad-bitboards
    uint8_t turn;           // side to move
    uint8_t ply;            // half-move count
    uint8_t ca;             // castling rights
    uint8_t ep;             // en-passent square
    struct undo_t hist[MM]; // history of board states
};

void parse(const char *fen, struct board_t *board);

uint64_t pawns(struct board_t *board);
uint64_t knights(struct board_t *board);
uint64_t bishops(struct board_t *board);
uint64_t rooks(struct board_t *board);
uint64_t queens(struct board_t *board);
uint64_t kings(struct board_t *board);
uint64_t all(struct board_t *board);

void flip(struct board_t *board);

void make(struct board_t *board, const uint16_t move);
void unmake(struct board_t *board, const uint16_t move);

extern struct board_t game[512];
extern struct board_t *board;

#endif /* BOARD_H */
