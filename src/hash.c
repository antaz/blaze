#include "hash.h"
#include "bitboard.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "search.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t piece_hash[PC][SQ];
static uint64_t ep_hash[9];
static uint64_t ca_hash[52];
static uint64_t stm_hash;

struct entry_t *table;
static int size_tt;

static uint64_t prng()
{
	static uint64_t next = 1;

	next = next * 1103515245 + 12345;
	return next;
}

void init_hash()
{
	for (int i = 0; i < PC; i++) {
		for (int j = 0; j < SQ; j++) {
			piece_hash[i][j] = prng();
		}
	}

	for (int i = 0; i <= 8; i++) {
		ep_hash[i] = prng();
	}

	for (int i = 0; i < 16; i++) {
		ca_hash[i] = prng();
	}

	stm_hash = prng();
}

uint64_t zobrist(struct board_t *board)
{
	uint64_t hash = 0ULL;

	uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
	uint64_t ours = board->bb[0];
	uint64_t theirs = all ^ ours;

	// hashing pieces
	// theirs
	for (uint64_t p =
		 (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & theirs;
	     p; p &= p - 1) {
		hash ^= piece_hash[PAWN][bsf(p)];
	}
	for (uint64_t p =
		 (~board->bb[1] & board->bb[2] & ~board->bb[3]) & theirs;
	     p; p &= p - 1) {
		hash ^= piece_hash[KNIGHT][bsf(p)];
	}
	for (uint64_t p = (board->bb[1] & board->bb[2]) & theirs; p;
	     p &= p - 1) {
		hash ^= piece_hash[BISHOP][bsf(p)];
	}
	for (uint64_t p =
		 (~board->bb[1] & ~board->bb[2] & board->bb[3]) & theirs;
	     p; p &= p - 1) {
		hash ^= piece_hash[ROOK][bsf(p)];
	}
	for (uint64_t p = (board->bb[1] & board->bb[3]) & theirs; p;
	     p &= p - 1) {
		hash ^= piece_hash[QUEEN][bsf(p)];
	}
	for (uint64_t p = (board->bb[2] & board->bb[3]) & theirs; p;
	     p &= p - 1) {
		hash ^= piece_hash[KING][bsf(p)];
	}

	hash = vflip(hash);

	// ours
	for (uint64_t p =
		 (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & board->bb[0];
	     p; p &= p - 1) {
		hash ^= piece_hash[PAWN][bsf(p)];
	}
	for (uint64_t p =
		 (~board->bb[1] & board->bb[2] & ~board->bb[3]) & board->bb[0];
	     p; p &= p - 1) {
		hash ^= piece_hash[KNIGHT][bsf(p)];
	}
	for (uint64_t p = (board->bb[1] & board->bb[2]) & board->bb[0]; p;
	     p &= p - 1) {
		hash ^= piece_hash[BISHOP][bsf(p)];
	}
	for (uint64_t p =
		 (~board->bb[1] & ~board->bb[2] & board->bb[3]) & board->bb[0];
	     p; p &= p - 1) {
		hash ^= piece_hash[ROOK][bsf(p)];
	}
	for (uint64_t p = (board->bb[1] & board->bb[3]) & board->bb[0]; p;
	     p &= p - 1) {
		hash ^= piece_hash[QUEEN][bsf(p)];
	}
	for (uint64_t p = (board->bb[2] & board->bb[3]) & board->bb[0]; p;
	     p &= p - 1) {
		hash ^= piece_hash[KING][bsf(p)];
	}

	// hashing ep square
	hash ^= ep_hash[board->ep];

	// hash castling
	hash ^= ca_hash[board->ca];

	// hash for color reversed
	if (board->stm == BLACK)
		hash ^= stm_hash;

	return hash;
}

void init_table(int size)
{
	free(table);

	size_tt = (size / sizeof(struct entry_t)) - 1;
	table = (struct entry_t *)calloc(size_tt + 1, sizeof(struct entry_t));

#ifdef DEBUG
	printf("Size of entry is: %lu\n", sizeof(struct entry_t));
	printf("Initialized hashtable with %d entries\n", size_tt);
#endif
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
	assert(depth > 0);
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
