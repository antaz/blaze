#include <stdio.h>
#include "string.h"
#include "definitions.h"
#include "functions.h"
#include <sys/time.h>

#define INPUTBUFFER 1000

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    return te.tv_sec * 1000LL + te.tv_usec/1000; // caculate milliseconds
}

void go(Board *board, Search *s, char * line) {
	PV pv;
	s->nodes = 0;
	s->stop = 0;
	
	int depth = -1, movestogo = 30,movetime = -1;
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
		time -= 50;	
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
