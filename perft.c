#include <stdio.h>
#include "definitions.h"

long leafNodes;

void perft(Board *board, int depth) {

	if(depth == 0) {
        leafNodes++;
        return;
    }	

    MoveList list[1];
    generateMoves(board,list);
      
    int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {	
       
        if ( !makeMove(board,list->moves[MoveNum]))  {
            continue;
        }
        perft(board, depth - 1);
        takeMove(board);
    }

    return;
}


void perftTest(Board *board, int depth) {
	printBoardSAN(board);
	printf("\nStarting Test To Depth:%d\n",depth);	
	leafNodes = 0;
	
	MoveList list[1];
	generateMoves(board,list);

	Move move;
	int MoveNum = 0;
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        move = list->moves[MoveNum];
        if ( !makeMove(board,move))  {
            continue;
        }
        long cumnodes = leafNodes;
        perft(board, depth - 1);
        takeMove(board);        
        long oldnodes = leafNodes - cumnodes;
        printf("move %d : ",MoveNum+1);
	moveSAN(move);
	printf(" : %ld\n", oldnodes);
    }
	
	printf("\nTest Complete : %ld nodes visited\n",leafNodes);

    return;
}
