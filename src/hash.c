#include "hash.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "search.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t prng()
{
	static uint64_t state = 0x123456789ABCDEF;
	state ^= state >> 12;
	state ^= state << 25;
	state ^= state >> 27;
	return state * 0x2545F4914F6CDD1D;
}

void init_hash()
{
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < PC; j++) {
			for (int k = 0; k < SQ; k++) {
				piece_hash[i][j][k] = prng();
			}
		}
	}

	for (int i = 0; i <= 8; i++) {
		ep_hash[i] = prng();
	}

	for (int i = 0; i < 52; i++) {
		ca_hash[i] = prng();
	}

	stm_hash = prng();
}

void init_table(int size)
{
	free(table);

	size_tt = (size / sizeof(struct entry_t)) - 1;
	table = (struct entry_t *)malloc(size);
}

void store(uint64_t hash, int depth, int nodes, int score, uint16_t move,
	   int flag)
{
	struct entry_t *entry = &table[hash & size_tt];

	entry->hash = hash;
	entry->depth = depth;
	entry->nodes = nodes;
	entry->score = score;
	entry->flag = flag;
	entry->move = move;
}

struct entry_t *probe(uint64_t hash)
{
	struct entry_t *entry = &table[hash & size_tt];

	if (entry->hash == hash)
		return entry;

	return NULL;
}

int probepv(struct board_t *board, struct pv_t *pv, int depth)
{
	struct entry_t *entry;
	struct move_t moves[256];

	for (int i = 1; i <= depth; i++) {
		if ((entry = probe(board->hash)) == NULL)
			break;

		int count = gen(board, moves);

		for (int j = 0; j < count; j++) {
			if (legal(board, moves[j].data))
				continue;

			if (moves[j].data == entry->move) {
				make(board, moves[j].data);

				pv->moves[pv->count] = moves[j].data;
				pv->count++;
				break;
			}
		}
	}

	for (int i = pv->count - 1; i >= 0; i--) {
		take(board, pv->moves[i]);
	}
	return 0;
}
