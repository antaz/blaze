#include "definitions.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

#include "board.h"

U64 pieceHash[13][120];
U64 turnHash;
U64 castleHash[16];

static U64 rand64()
{
        U64 r = 0;
        int i;
        for (i = 0; i < 64; i++) {
                r = r * 2 + rand() % 2;
        }
        return r;
}

void initZobrist()
{
        int i, j;

        for (i = 0; i < 13; i++) {
                for (j = 0; j < 120; j++) {
                        pieceHash[i][j] = rand64();
                }
        }

        turnHash = rand64();

        for (i = 0; i < 16; i++) {
                castleHash[i] = rand64();
        }
}

U64 generateHash(struct board_t *board)
{
        int square;
        int piece;
        U64 zobrist = 0;

        for (square = 0; square < SQNUM; square++) {
                piece = board->pieces[square];
                if (piece != OFFBOARD && piece != EMPTY) {
                        zobrist ^= pieceHash[piece][square];
                }
        }

        if (board->turn == WHITE) {
                zobrist ^= turnHash;
        }

        if (board->enPassant != NO_SQ) {
                zobrist ^= pieceHash[EMPTY][board->enPassant];
        }

        zobrist ^= castleHash[board->castling];

        return zobrist;
}

// initialization of TT

const int TTSize = 0x100000 * 2;

void clearTTable(TTable *table)
{
        TTEntry *entry;
        for (entry = table->table; entry < table->table + table->size;
             entry++) {
                entry->zobristHash = 0ULL;
                entry->depth = 0;
                entry->score = 0;
                entry->flags = 0;
        }
        table->newWrite = 0;
}
void initTTable(TTable *table)
{
        table->size = TTSize / sizeof(TTEntry);
        table->size -= 2;
        if (table->table != NULL) {
                free(table->table);
        }
        table->table = (TTEntry *)malloc(table->size * sizeof(TTEntry));
        clearTTable(table);
        // printf("init of TTable complete with %d entries\n", table->size);
}

int probeTT(struct board_t *board, Move *move)
{
        int index = board->zobristHash % board->table->size;

        if (board->table->table[index].zobristHash == board->zobristHash) {
                move->from = board->table->table[index].move.from;
                move->to = board->table->table[index].move.to;
                return 1;
        }

        return 0;
}

void storeTT(struct board_t *board, Move move)
{
        int index = board->zobristHash % board->table->size;
        board->table->table[index].move = move;
        board->table->table[index].zobristHash = board->zobristHash;
}
