#include <assert.h>
#include <stdio.h>

#include "movegen.h"

/* printMoveList: print `list` of moves */
void printMoveList(MoveList *list)
{
        int i;
        printf("\nMove list: \n");
        for (i = 0; i < list->count; i++) {
                printf("%d. ", i + 1);
                printMove(list->moves[i]);
                printf(" %d", list->moves[i].score);
                printf("\n");
        }
}
