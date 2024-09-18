#include "perft.h"
#include "gen.h"
#include "hash.h"
#include "move.h"
#include <stdint.h>
#include <stdio.h>

int collisions = 0;
int insertions = 0;

uint64_t perft(struct board_t *board, int depth)
{
	struct move_t moves[320];
	int count = 0;
	uint64_t nodes = 0;

	count = gen_legal(board, moves);

	if (depth == 1)
		return count;
	else if (depth == 0)
		return 1;

	struct entry_t *entry = probe(board->hash);
	if (entry != NULL && entry->depth == depth)
		return entry->nodes;

	for (int i = 0; i < count; ++i) {
		make(board, moves[i].data);
		nodes += perft(board, depth - 1);
		take(board, moves[i].data);
	}

	store(board->hash, depth, nodes, 0, 0, 0);

	return nodes;
}

void perft_divide(struct board_t *board, int depth)
{
	struct move_t moves[320];
	int count = 0;
	uint64_t nodes = 0;
	uint64_t total_nodes = 0;

	count = gen_legal(board, moves);

	for (int i = 0; i < count; ++i) {
		make(board, moves[i].data);
		nodes = perft(board, depth - 1);
		take(board, moves[i].data);
		printf("%s: %lu\n", m2uci(moves[i].data, board->stm), nodes);
		total_nodes += nodes;
	}

	printf("\nNodes searched: %lu\n", total_nodes);
}
