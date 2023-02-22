#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "move.h"
#include "search.h"

#define NAME "Blaze"
#define AUTHOR "Dr. Sara Tancredi"
#define INPUTBUFFER 2400

#ifdef WIN32
#include <Windows.h>
#include <io.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

void go(struct board_t *board, Search *s, char *line);

void position(char *lineIn, struct board_t *board);
void uci_loop(struct board_t *board, Search *search);

#endif 
