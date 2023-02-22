#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "move.h"

#define MAX 256

// pieces
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };

// Move list sructure and store the generated moves
typedef struct {
        struct move_t moves[MAX];
        int count;
} MoveList;

void printMoveList(MoveList *list);

#endif
