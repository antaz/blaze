#include <stdio.h>
#include <assert.h>
#include "definitions.h"
#include "functions.h"

void moveSAN(Move move) {
	char *pieces = ".pnbrq";
	int promoted = move.promoted;
	assert(move.from != OFFBOARD);
	assert(move.to != OFFBOARD);
	if(promoted) {
		printf("%c%d%c%d%c", (FILE120(move.from) + 1) + 'a', RANK120(move.from) + 1, (FILE120(move.to) + 1) + 'a', RANK120(move.to) + 1,pieces[promoted % 6]);
	} else {
		printf("%c%d%c%d", (FILE120(move.from) + 1) + 'a', RANK120(move.from) + 1, (FILE120(move.to) + 1) + 'a', RANK120(move.to) + 1);
	}
}
