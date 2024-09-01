#include "bitboard.h"
#include "board.h"
#include <stdint.h>

// clang-format off
const int PST[8][64] =
{
{  0,  0,  0,  0,  0,  0,  0,  0, /* empty */
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0},
{  0,  0,  0,  0,  0,  0,  0,  0, /* pawn */
   5,  5,  5,-20,-20,  5,  5,  5,
   5, -5, -5,  0,  0, -5, -5,  5,
   0,  0,  0, 12, 12,  0,  0,  0,
  10, 10, 10, 16, 16, 10, 10, 10,
  30, 30, 30, 40, 40, 30, 30, 30,
 100,100,100,100,100,100,100,100,
   0,  0,  0,  0,  0,  0,  0,  0},
{ -5, -5,  0,  0,  0,  0, -5, -5,  /* knight */
  -5,  0, 10, 10, 10, 10,  0, -5,
   0, 10, 15, 15, 15, 15, 10,  0,
   0, 10, 15, 20, 20, 15, 10,  0,
   0, 10, 15, 20, 20, 15, 10,  0,
   0, 10, 15, 15, 15, 15, 10,  0,
  -5,  0, 10, 10, 10, 10,  0, -5,
  -5, -5,  0,  0,  0,  0, -5, -5},
{ -5, -5,  0,  0,  0,  0, -5, -5,  /* bishop */
  -5,  0, 10, 10, 10, 10,  0, -5,
  -5, 10, 15, 15, 15, 15, 10, -5,
  -5, 10, 15, 20, 20, 15, 10, -5,
  -5, 10, 15, 20, 20, 15, 10, -5,
  -5, 10, 15, 15, 15, 15, 10, -5,
  -5,  0, 10, 10, 10, 10,  0, -5,
  -5, -5,  0,  0,  0,  0, -5, -5},
{  0, -5, -5, 10, 10, -5, -5,  0, /* rook */
   0,  0,  0, 10, 10,  0,  0,  0,
   0,  0,  0, 10, 10,  0,  0,  0,
   0,  0,  0, 10, 10,  0,  0,  0,
   0,  0,  0, 10, 10,  0,  0,  0,
   0,  0,  0, 10, 10,  0,  0,  0,
  20, 20, 20, 20, 20, 20, 20, 20,
  10, 10, 10, 10, 10, 10, 10, 10},
{ -4, -4, -4, -4, -4, -4, -4, -4,  /* queen */
  -4,  0,  0,  0,  0,  0,  0, -4,
  -4,  0,  4,  4,  4,  4,  0, -4,
  -4,  0,  4,  8,  8,  4,  0, -4,
  -4,  0,  4,  8,  8,  4,  0, -4,
  -4,  0,  4,  4,  4,  4,  0, -4,
  -4,  0,  0,  0,  0,  0,  0, -4,
  -4, -4, -4, -4, -4, -4, -4, -4},
{ -5, 25, 15,-10,  0,-10, 30, -5, /* king middlegame */
  -5, -5, -5, -5, -5, -5, -5, -5,
  -5, -5, -5, -5, -5, -5, -5, -5,
  -5, -5, -5, -5, -5, -5, -5, -5,
  -5, -5, -5, -5, -5, -5, -5, -5,
  -5, -5, -5, -5, -5, -5, -5, -5,
  -5, -5, -5, -5, -5, -5, -5, -5,
  -5, -5, -5, -5, -5, -5, -5, -5},
{  0, -5, -5,  0,  0,  0, -5,  0, /* king endgame */
   0,  5,  5,  5,  5,  5,  5,  0,
   0,  5, 10, 10, 10, 10,  5,  0,
   0,  5, 10, 15, 15, 10,  5,  0,
   0,  5, 10, 15, 15, 10,  5,  0,
   0,  5, 10, 10, 10, 10,  5,  0,
   0,  5,  5,  5,  5,  5,  5,  0,
   0,  0,  0,  0,  0,  0,  0,  0}
};

const int StaticValue[8] = {0, 100, 300, 300, 500, 950, 0, 0};

int eval(struct board_t *board)
{

    int eval = 0;
    uint64_t pawn = (board->bb[1] & ~board->bb[2] & ~board->bb[3]);
    uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]);
    uint64_t bishop = (board->bb[1] & board->bb[2]);
    uint64_t rook = (~board->bb[1] & ~board->bb[2] & board->bb[3]);
    uint64_t queen = (board->bb[1] & board->bb[3]);
    uint64_t king = (board->bb[2] & board->bb[3]);
    int phase = popcnt(pawn | knight | bishop | rook | queen | king);

    for(int i = 0; i < 2; ++i) {
        for (uint64_t p = (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & board->bb[0]; p; p &= p - 1) {
            eval += PST[PAWN][bsf(p)] + StaticValue[PAWN];
        }
        for (uint64_t p = (~board->bb[1] & board->bb[2] & ~board->bb[3]) & board->bb[0]; p; p &= p - 1) {
            eval += PST[KNIGHT][bsf(p)] + StaticValue[KNIGHT];
        }
        for (uint64_t p = (board->bb[1] & board->bb[2]) & board->bb[0]; p; p &= p - 1) {
            eval += PST[BISHOP][bsf(p)] + StaticValue[BISHOP];
        }
        for (uint64_t p = (~board->bb[1] & ~board->bb[2] & board->bb[3]) & board->bb[0]; p; p &= p - 1) {
            eval += PST[ROOK][bsf(p)] + StaticValue[ROOK];
        }
        for (uint64_t p = (board->bb[1] & board->bb[3]) & board->bb[0]; p; p &= p - 1) {
            eval += PST[QUEEN][bsf(p)] + StaticValue[QUEEN];
        }
        uint64_t ksq = bsf((board->bb[2] & board->bb[3]) & board->bb[0]);
        eval += (PST[KING][ksq] * phase + PST[KING + 1][ksq]*(16 - phase))/16;
        flip(board);
        eval = -eval;
    }

    return eval;
}
