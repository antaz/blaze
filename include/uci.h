#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "search.h"

int senduci(char *buffer);
int pvinfo(struct search_t *data, struct pv_t *pv, int score);
void loop(struct board_t *board);

#endif
