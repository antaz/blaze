#include "board.h"
#include "hash.h"
#include "uci.h"
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>

int main()
{
	init_hash();
	init_table(0x100000 * 16);
	struct board_t board = {
	    .bb = {0}, .stm = WHITE, .ep = NOSQ, .ca = 0, .hist = {{0}}};
	parse(&board,
	      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	loop(&board);

	free(table);
	return EXIT_SUCCESS;
}
