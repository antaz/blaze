#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>

enum { WHITE, BLACK, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

/* main structure to hold the state of the board */
struct board_t {
        uint64_t piece[8]; // piece bitboards
        bool turn;         // side to move
        uint8_t ply;       // half-move counter
        uint8_t castle;    // castling rights
        uint8_t ep;        // en-passent square
};

/* parse: parses `fen` (Forsyth–Edwards Notation) string into `board` structure
 */
void parse(const char *fen, struct board_t *board);

/* generate: gnerates pseudo-legal moves */
void generate(const struct board_t *board, uint16_t *moves);

uint64_t checks(const uint64_t piece[], bool color);

uint64_t danger(const uint64_t piece[], bool color);

#endif /* BOARD_H */
