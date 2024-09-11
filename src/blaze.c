#include "board.h"
#include "hash.h"
#include "uci.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// initialize interrupt signal
volatile sig_atomic_t interrupt = 0;

static void handle(int sig)
{
    if (sig == SIGINT || sig == SIGTERM) {
        interrupt = 1;
    }
}

int main()
{
    // initialize a sigaction
    struct sigaction sa = {0};

    sa.sa_handler = handle;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    // attempt to handle SIGINT and exit on failure
    if (sigaction(SIGINT, &sa, NULL) < 0)
        return EXIT_FAILURE;

    init_hash();
    init_table(2 * 1024 * 1024);
    struct board_t board = {
        .bb = {0}, .stm = WHITE, .ep = NOSQ, .ca = 0, .hist = {0}};
    parse(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    loop(&board);

    free(table);
    return EXIT_SUCCESS;
}
