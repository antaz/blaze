#include <stdio.h>
#include <string.h>
#include "uci.h"
#include "board.h"
#include "perft.h"

#define SIZE 1024

void loop(struct board_t *board) {
    char buf[1024];

    while (1) {
        if(fgets(buf, 1024, stdin) != NULL){
            if(strcmp("uci\n", buf) == 0) {
                printf("uciok\n");
            } else if(strcmp("ready\n", buf) == 0) {
                printf("readyok\n");
            } else if(strncmp("go perft", buf, 7) == 0) {
                char depth;
                sscanf(buf, "go perft %c", &depth);
                perft_divide(board, depth - '0');
            }
        }
    }
}
