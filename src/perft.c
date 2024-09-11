#include "perft.h"
#include "gen.h"
#include "move.h"
#include <stdio.h>

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

	for (int i = 0; i < count; ++i) {
		make(board, moves[i].data);
		nodes += perft(board, depth - 1);
		take(board, moves[i].data);
	}

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
