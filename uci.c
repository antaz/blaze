#include <stdio.h>
#include "string.h"
#include "definitions.h"
#include "functions.h"

#define INPUTBUFFER 1000

#ifdef WIN32
#include <Windows.h>
#include <io.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

long long current_timestamp() {
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval te;
	gettimeofday(&te, NULL);
	return te.tv_sec * 1000 + te.tv_usec / 1000;
#endif
}

void go(Board *board, Search *s, char * line) {
	PV pv;
	s->nodes = 0;
	s->stop = 0;
	
	int depth = -1, movestogo = 40,movetime = -1;
	int time = -1, inc = 0;
	char *ptr = NULL;
	s->timeset = 0;

	if((ptr = strstr(line,"INF")));

	if((ptr = strstr(line,"binc")) && board->turn == BLACK)
		inc = atoi(ptr + 5);
	if((ptr = strstr(line,"winc")) && board->turn == WHITE)
		inc = atoi(ptr + 5);
	if((ptr = strstr(line,"wtime")) && board->turn == WHITE)
		time = atoi(ptr + 6);
	if((ptr = strstr(line,"btime")) && board->turn == BLACK)
		time = atoi(ptr + 6);
	if((ptr = strstr(line,"movestogo")))
		movestogo = atoi(ptr + 10);
	if((ptr = strstr(line,"movetime")))
		movetime = atoi(ptr + 9);
	
	if((ptr = strstr(line,"depth")))
		depth = atoi(ptr + 6);

	if(movetime != -1) {
		time = movetime;
		movestogo = 1;
	}

	s->starttime = current_timestamp();
	s->depth = depth;

	if(time != -1) {
		s->timeset = 1;
		time /= movestogo;
		time -= 20;	
		s->stoptime = s->starttime + time + inc;
	}

	if(depth == -1)
		s->depth = MAXDEPTH;

	printf("time:%d start:%lld stop:%lld depth:%d timeset:%d\n",time, s->starttime, s->stoptime, s->depth, s->timeset);
		
	search(board, s, &pv);
}

void position(char *lineIn, Board *board) {
	char *ptrChar = lineIn + 9;
	Move move;

	lineIn = ptrChar;

	if(strncmp(lineIn, "startpos", 8) == 0) {
		parseFEN(START_FEN, board);
	} else {
		ptrChar = strstr(lineIn, "fen");
		if(ptrChar == NULL) {
			parseFEN(START_FEN, board);
		} else {
			ptrChar += 4;
			parseFEN(ptrChar, board);
		}
	}

	ptrChar = strstr(lineIn, "moves");

	if(ptrChar != NULL) {
		ptrChar += 6;
		while(*ptrChar) {
			if(!parseMove(board, ptrChar, &move))
				break;
			makeMove(board, move);
			board->ply = 0;
			while(*ptrChar && *ptrChar != ' ')
				ptrChar++;
			ptrChar++;
		}
	}
	//printBoardSAN(board);
}
void uci_loop(Board *board, Search *search) {

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	char line[INPUTBUFFER];
	while(1) {
		memset(&line[0], 0, sizeof(line));
		fflush(stdout);
		if(!fgets(line, INPUTBUFFER, stdin))
			continue;
		if(line[0] == '\n')
			continue;
		if(!strncmp(line, "isready", 7)) {
			printf("readyok\n");
			continue;
		} else if (!strncmp(line, "position", 8)) {
			position(line, board);
		} else if (!strncmp(line, "ucinewgame", 10)) {
			position("position startpos\n", board);
		} else if (!strncmp(line, "go", 2)) {
			go(board, search, line);
		} else if (!strncmp(line, "quit", 4)) {
			break;
		} else if (!(strncmp(line, "stop", 4))) {
			search->stop = 1;
		} else if (!strncmp(line, "uci", 3)) {
			printf("id name %s\n", NAME);
			printf("id author %s\n", AUTHOR);
			printf("uciok\n");
		}
	}	
}

int inputWaiting()
{
#ifndef WIN32
	struct timeval tv;
	fd_set readfds;

	FD_ZERO (&readfds);
	FD_SET (fileno(stdin), &readfds);
	tv.tv_sec=0; tv.tv_usec=0;
	select(16, &readfds, 0, 0, &tv);

	return (FD_ISSET(fileno(stdin), &readfds));
#else
	static int init = 0, pipe;
	static HANDLE inh;
	DWORD dw;

	if (!init) {
		init = 1;
		inh = GetStdHandle(STD_INPUT_HANDLE);
		pipe = !GetConsoleMode(inh, &dw);
		if (!pipe) {
			SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
			FlushConsoleInputBuffer(inh);
		}
	}
	if (pipe) {
		if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL))
			return 1;
		return dw;
	} else {
		GetNumberOfConsoleInputEvents(inh, &dw);
		return dw <= 1 ? 0 : dw;
	}
#endif
}

void readInput(Search *search) {
	int bytes;
	char input[256] = "", *endc;

	if(inputWaiting()) {
		search->stop = 1;
		do {
			bytes=read(fileno(stdin), input, 256);
		} while (bytes<0);
		endc = strchr(input, '\n');
		if (endc)
			*endc = 0;

		if (strlen(input) > 0) {
			if (!strncmp(input, "quit", 4))	{
				//search->quit = 1;
			}
		}
		return;
	}
}