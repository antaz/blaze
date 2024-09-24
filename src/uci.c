#include "uci.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "perft.h"
#include "search.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// search thread
static pthread_t worker;

// search driver
extern struct search_t driver;

#define BUF_SIZE 65536
struct tc_t tc_data = {.movestogo = MOVESTOGO};

int senduci(char *buffer)
{
	fprintf(stdout, "%s\n", buffer);
	return 0;
}

int pvinfo(struct search_t *data, struct pv_t *pv, int score)
{
	char buf[4096];
	struct timespec now;

	clock_gettime(CLOCK_MONOTONIC, &now);
	long long time = (now.tv_sec - data->start.tv_sec) * 1000 +
			 (now.tv_nsec - data->start.tv_nsec) / 1000000;
	long long nps = time > 0 ? (data->nodes * 1000) / time : 0;

	int s = data->stm;
	char pv_str[4096] = {0}; // Initialize to empty string

	for (int i = 0; i < pv->count; i++) {
		strncat(pv_str, m2uci(pv->moves[i], s),
			sizeof(pv_str) - strlen(pv_str) - 1);
		strncat(pv_str, " ", sizeof(pv_str) - strlen(pv_str) - 1);
		s ^= BLACK;
	}

	char score_str[10] = {0};
	if (abs(score) < INF - 2000) {
		snprintf(score_str, sizeof(score_str), "cp %d", score);
	} else {
		if (score > 0) {
			snprintf(score_str, sizeof(score_str), "mate %d",
				 (INF - score) / 2 + 1);
		} else {
			snprintf(score_str, sizeof(score_str), "mate %d",
				 -(INF + score) / 2 - 1);
		}
	}

	sprintf(buf,
		"info depth %d score %s time %lld nodes %lld nps %lld pv %s",
		data->depth, score_str, time, data->nodes, nps, pv_str);

	return senduci(buf);
}

void loop(struct board_t *board)
{
	char buf[BUF_SIZE];

	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

	for (;;) {
		if (!fgets(buf, BUF_SIZE, stdin)) {
			break;
		}

		if (!strncmp("isready", buf, 7)) {
			printf("readyok\n");
		} else if (!strncmp("go perft", buf, 8)) {
			perft_divide(board, atoi(buf + 9));
		} else if (!strncmp("ucinewgame", buf, 10)) {
			parse(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/"
				     "RNBQKBNR w KQkq - 0 1");
		} else if (!strncmp("uci", buf, 3)) {
			printf("id name Blaze 0.1\n");
			printf("id author Antar Azri\n");
			printf("uciok\n");
		} else if (!strncmp("position", buf, 8)) {
			if (strstr(buf, "startpos")) {
				parse(board, "rnbqkbnr/pppppppp/8/8/8/8/"
					     "PPPPPPPP/RNBQKBNR w KQkq "
					     "- 0 1");
			} else if (strstr(buf, "fen")) {
				parse(board, buf + 13);
			}

			char *moves = strstr(buf, "moves");
			if (moves) {
				char *move = strtok(moves + 6, " \n");
				while (move != NULL) {
					struct move_t moves[320];
					int count = 0;
					count = gen_legal(board, moves);
					char *str_m;

					for (int i = 0; i < count; ++i) {
						str_m = m2uci(moves[i].data,
							      board->stm);
						if (!strcmp(str_m, move)) {
							make(board,
							     moves[i].data);
							break;
						}
					}

					move = strtok(NULL, " \n");
				}
			}
		} else if (!strncmp("go", buf, 2)) {
			tc_data.depth = -1;
			tc_data.timeset = -1;
			char *token = NULL;

			if ((token = strstr(buf, "infinite"))) {
				// do an infinite search
			}
			if ((token = strstr(buf, "ponder"))) {
				// ponder a move
			}
			if ((token = strstr(buf, "wtime"))) {
				tc_data.time[WHITE] = atoi(token + 6);
				tc_data.timeset = 1;
			}
			if ((token = strstr(buf, "btime"))) {
				tc_data.time[BLACK] = atoi(token + 6);
				tc_data.timeset = 1;
			}
			if ((token = strstr(buf, "winc"))) {
				tc_data.inc[WHITE] = atoi(token + 5);
			}
			if ((token = strstr(buf, "binc"))) {
				tc_data.inc[BLACK] = atoi(token + 5);
			}
			if ((token = strstr(buf, "movestogo"))) {
				tc_data.movestogo = atoi(token + 10);
			}
			if ((token = strstr(buf, "depth"))) {
				tc_data.depth = atoi(token + 6);
			}
			if ((token = strstr(buf, "nodes"))) {
				tc_data.nodes = atoi(token + 6);
			}
			if ((token = strstr(buf, "movetime"))) {
				tc_data.movetime = atoi(token + 9);
				tc_data.timeset = 1;
			}

			if (pthread_create(&worker, NULL, deepen,
					   (void *)board) != 0) {
				perror("Error: creating search thread");
				exit(EXIT_FAILURE);
			}

		} else if (!strncmp("stop", buf, 4)) {
			// stop search
			driver.stop = 1;

			if (pthread_join(worker, NULL) != 0) {
				exit(EXIT_FAILURE);
			}

		} else if (!strncmp("quit", buf, 4)) {
			driver.stop = 1;
			// TODO: this conflicts with uci stop
			if (pthread_join(worker, NULL) != 0) {
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
}
