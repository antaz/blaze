#ifndef HASH_H
#define HASH_H

#include "board.h"
#include "search.h"
#include <stdint.h>

extern struct entry_t *table;

// hash move flags
enum { EXACT, UPBOUND, LOBOUND };

// hash table entry
struct entry_t {
	uint64_t hash;
	uint16_t nodes; // TODO: this is not enough for node count
	int16_t score;
	uint16_t move;
	uint8_t depth;
	uint8_t flag;
};

void init_hash();
uint64_t zobrist(struct board_t *board);
void init_table(int size);
void store(uint64_t hash, int depth, int nodes, int score, uint16_t move,
	   int flag);
struct entry_t *probe(uint64_t hash);
int probepv(struct board_t *board, struct pv_t *pv, int depth);

#endif
