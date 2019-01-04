#include <stdio.h>
#include "definitions.h"
#include "functions.h"

int scores[13] = {0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};

int mvvlva(int victim, int attacker) {
	return scores[victim] + 6 - (scores[attacker] / 100);
}

int compareMoves(const Move *m1, const Move *m2) {
	if (m1->score > m2->score) return -1;
    if (m1->score < m2->score) return +1;
    return 0;
}

int moveScore(Board *board, Move move) {
	int score = mvvlva(move.captured, board->pieces[move.from]);

	if(move.captured) {
		score += 1000000;
	} else {

		if (move.from == board->kill[0][board->ply].from && move.to == board->kill[0][board->ply].to) {
			score = 900000;
		} else if(move.from == board->kill[1][board->ply].from && move.to == board->kill[1][board->ply].to) {
			score = 800000;
		} else {
			score += board->his[board->pieces[move.from]][move.to];
		}
	}
	return score;
}