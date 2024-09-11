#ifndef HASH_H
#define HASH_H

#include "board.h"
#include "search.h"
#include <stdint.h>

// invalid tt hit
#define INVALID -445

// hash move flags
enum { EXACT, UPBOUND, LOBOUND };

extern uint64_t piece_hash[2][PC][SQ];
extern uint64_t ep_hash[8];
extern uint64_t ca_hash[52];
extern uint64_t stm_hash;

extern struct entry_t *table;
extern int size_tt;

// hash table entry
struct entry_t {
	uint64_t hash;
	int depth;
	int score;
	int flag;
	uint16_t move;
};

void init_hash();
void init_table(int size);
void store(uint64_t hash, int depth, int score, uint16_t move, int flag);
int probe(uint64_t hash, uint16_t *move);
int probepv(struct board_t *board, struct pv_t *pv, int depth);

#endif
