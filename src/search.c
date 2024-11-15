#include "search.h"
#include "board.h"
#include "eval.h"
#include "gen.h"
#include "hash.h"
#include "move.h"
#include "uci.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>

struct search_t driver;
static uint64_t movetime; // how long should we search

static void halt()
{
	// check if we should stop searching
	if (movetime != 0) {
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		uint64_t elapsed = (now.tv_sec - driver.start.tv_sec) * 1e3 +
				   (now.tv_nsec - driver.start.tv_nsec) / 1e6;
		if (elapsed >= movetime) {
			driver.stop = 1;
		}
	}
}

static int rep(struct board_t *board)
{
	int r = 0;
	for (int i = board->hply - board->fifty; i < board->hply; i++) {
		if (board->hist[i].hash == board->hash) {
			r++;
		}
	}
	return r;
}

static int quiesce(struct board_t *board, int alpha, int beta)
{
	if ((driver.nodes & 1023) == 0)
		halt();

	if (driver.stop)
		return 0;

	int score;
	driver.nodes++;

	score = eval(board);

	if (score >= beta)
		return beta;

	if (score > alpha)
		alpha = score;

	struct move_t moves[256];
	int count = capture(board, moves);
	for (int i = 0; i < count; i++) {
		if (legal(board, moves[i].data))
			continue;

		make(board, moves[i].data);
		score = -quiesce(board, -beta, -alpha);
		take(board, moves[i].data);

		if (driver.stop)
			return 0;

		if (score > alpha) {
			if (score >= beta)
				return beta;

			alpha = score;
		}
	}

	return alpha;
}

static int search(struct board_t *board, int alpha, int beta, int depth)
{
	int score = 0;
	int best = -INF;
	int oldalpha = alpha;
	int active = 0;
	struct move_t bestmove;

	if ((driver.nodes & 1023) == 0)
		halt();

	if (depth == 0) {
		return quiesce(board, alpha, beta);
	}

	driver.nodes++;

	if (board->ply && rep(board)) {
		return 0;
	}

	// probe hash table for hash move
	struct entry_t *entry = poll(board->hash);
	if (entry != NULL) {
		if (entry->draft >= depth) {
			// adjust mate score
			score = entry->score;
			if (entry->score >= INF - 64)
				score = entry->score - board->ply;
			if (entry->score <= -INF + 64)
				score = entry->score + board->ply;

			if (entry->flag == EXACT)
				return score;
			if ((entry->flag == LOBOUND) && (entry->score <= alpha))
				return alpha;
			if ((entry->flag == UPBOUND) && (entry->score >= beta))
				return beta;
		}
	}

	struct move_t moves[256];
	int count = gen(board, moves);

	// order the hash move to be searched first
	if (entry != NULL) {
		for (int i = 0; i < count; i++) {
			if (moves[i].data == entry->move) {
				struct move_t temp = moves[0];
				moves[0] = moves[i];
				moves[i] = temp;
				break;
			}
		}
	}

	for (int i = 0; i < count; i++) {
		if (legal(board, moves[i].data))
			continue;

		active++;
		make(board, moves[i].data);
		score = -search(board, -beta, -alpha, depth - 1);
		take(board, moves[i].data);

		if (driver.stop)
			return 0;

		if (score > best) {
			best = score;
			bestmove = moves[i];
			if (score > alpha) {
				if (score >= beta) {
					// adjust mate score
					score = beta;
					if (beta >= INF - 64)
						score = beta + board->ply;
					if (beta <= -INF + 64)
						score = beta - board->ply;

					save(board->hash, depth, driver.nodes,
					     score, bestmove.data, UPBOUND);
					return beta;
				}
				alpha = score;
			}
		}
	}

	if (board->fifty >= 100) {
		return 0;
	}

	if (!active) {
		if (check(board)) {
			return -INF + board->ply;
		} else {
			return 0;
		}
	}

	if (alpha != oldalpha) {
		// adjust mate score
		score = best;
		if (best >= INF - 64)
			score = best + board->ply;
		if (best <= -INF + 64)
			score = best - board->ply;
		save(board->hash, depth, driver.nodes, score, bestmove.data,
		     EXACT);
	} else {
		// adjust mate score
		score = alpha;
		if (alpha >= INF - 64)
			score = alpha + board->ply;
		if (alpha <= -INF + 64)
			score = alpha - board->ply;
		save(board->hash, depth, driver.nodes, score, bestmove.data,
		     LOBOUND);
	}
	return alpha;
}

void *deepen(void *args)
{
	struct board_t *board = (struct board_t *)args;
	int score;
	int alpha = -2 * INF, beta = 2 * INF;
	int max_depth = MAX_DEPTH;
	uint16_t bestmove = 0;
	movetime = 0;
	driver.stm = board->stm;
	driver.stop = 0;
	driver.nodes = 0;
	driver.depth = 1;
	board->ply = 0;

	// initialize time management
	clock_gettime(CLOCK_MONOTONIC, &driver.start);

	if (tc_data.timeset != -1) {
		if (!tc_data.movetime) {
			movetime =
			    tc_data.time[board->stm] / tc_data.movestogo +
			    tc_data.inc[board->stm];
			if (movetime > 20)
				movetime -= 20;
		} else {
			movetime = tc_data.movetime;
		}
	}

	if (tc_data.depth != -1)
		max_depth = tc_data.depth;

	// iterative deepening
	while (driver.depth <= max_depth) {

		struct pv_t pv = {0};
		score = search(board, alpha, beta, driver.depth);

		if (driver.stop)
			break;

		probepv(board, &pv, driver.depth);
		bestmove = pv.moves[0];
		pvinfo(&driver, &pv, score);
		driver.depth++;
	}

	printf("bestmove %s\n", m2uci(bestmove, driver.stm));
	pthread_exit(0);
}
