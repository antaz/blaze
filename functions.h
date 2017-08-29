/*
 * functions.h
 *
 *  Created on: Aug 27, 2017
 *      Author: Antar
 */


#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Macro functions
#define FR2SQ64(f, r) (((r) << 3) | (f))
#define FR2SQ120(f,r) (((r)+2)*10 + (f) + 1)
#define RANK(sq) ((sq) >> 3)
#define FILE(sq) ((sq) & 7)
#define RANK120(sq) ((sq) / 10)
#define FILE120(sq) ((sq) % 10)
#define SQ64TO120(sq) ((((sq) >> 3)+2)*10 + ((sq) & 7) + 1)

// ((x/8) + 2)*10 + (x + 7) + 1

// board.c
extern void clearBoard(Board *board);
extern void printBoard(Board *board);
extern void parseFEN(char *fen, Board *board);

// move.c
extern void moveSAN(Move *move);

#endif
