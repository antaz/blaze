#ifndef SEARCH_H
#define SEARCH_H

#define MATE 99999

// search driver
typedef struct {
        long long starttime, stoptime;
        int depth, timeset, movestogo, infinite, stop;
        long nodes;
} Search;


void readInput(Search *search);

void search(struct board_t *board, Search *search, PV *pv);

int count_nps(long nodes, long long time);

long long current_timestamp();

int inputWaiting();

#endif
