#ifndef BOARD_H
#define BOARD_H

#include "definitions.h"

/* main structure to hold the state of the board */
struct board_t {
        int pieces[SQNUM];
        int pieceCount[13];
        int pieceList[13][10];
        int kingSquare[2];
        int turn, enPassant, fiftyMove;
        int ply, hisPly, castling;
        int material[2];
        Undo history[MAX];

        int his[13][SQNUM];
        Move kill[2][MAXDEPTH];

        U64 zobristHash;
        TTable table[1];
};

void parseFEN(char *fen, struct board_t *board);
void clearBoard(struct board_t *board);
void updatePieceList(struct board_t *board);
void printBoard(struct board_t *board);
void printMoveList(MoveList *list);
void printPV(PV *pv);

#endif /* BOARD_H */
