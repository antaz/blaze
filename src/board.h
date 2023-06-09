#ifndef BOARD_H
#define BOARD_H

#include "type.h"
#include <stdint.h>

/* non-recoverable information */
struct undo_t {
        uint8_t ply; // half-move count
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

/** @brief FEN (Forsyth-Edwards Notation) parser
 *  @param fen the FEN string
 *  @param board board state
 *  @return void
 */
void parse(const char *fen, struct board_t *board);

/** @brief flip the board with respect to the side to move
 *  @param board board state
 *  @return void
 */
void flip(struct board_t *board);

/** @brief make the move on the board
 *  @param board board state
 *  @param move the move to be played
 *  @return void
 */
void make(struct board_t *board, const uint16_t *move);

/** @brief unmake the last move played on the board
 *  @param board board state
 *  @return void
 */
void unmake(struct board_t *board);

#endif /* BOARD_H */
