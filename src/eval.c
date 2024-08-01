#include "board.h"
#include <stdlib.h>
#include <time.h>

int eval(struct board_t *board) {
    srand(time(NULL));
    return (rand() % 201) - 100;
}
