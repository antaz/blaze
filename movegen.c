// Move generation functions

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "definitions.h"
#include "functions.h"

const int knightDir[8] = { -8, -19, -21, -12, 8, 19, 21, 12 };
const int rookDir[4] = { -1, -10, 1, 10 };
const int bishopDir[4] = { -9, -11, 11, 9 };
const int kingDir[8] = { -1, -10, 1, 10, -9, -11, 11, 9 };

int dirs[4][8]

void generateMove(Board *board) {
	// TODO create the moves list structure
	// loop through each piece on the board
	// if it is pawn -> generatePawnMoves()
	// if not
	// find each possible move and add it to the move list
}
int * vector = dirs[<piece>];

while (*vector != 0) {
    int nextsq = currsq + *vector;
    
    while (board[nextsq] != OFFBOARD) {
        AddMove(currsq, nextsq, <whatever>);
        
        if (board[nextsq] != EMPTY || !slide[<piece>]) 
            break;
        
        nextsq += *vector;
    }
    
    *vector++;
}
