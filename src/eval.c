#include "bitboard.h"
#include "board.h"

int eval(struct board_t *board)
{

    uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
    uint64_t ours = board->bb[0];
    uint64_t theirs = all ^ ours;
    uint64_t pawn = (board->bb[1] & ~board->bb[2] & ~board->bb[3]);
    uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]);
    uint64_t bishop = (board->bb[1] & board->bb[2]);
    uint64_t rook = (~board->bb[1] & ~board->bb[2] & board->bb[3]);
    uint64_t queen = (board->bb[1] & board->bb[3]);
    uint64_t king = (board->bb[2] & board->bb[3]);

    // material score
    return 100 * (popcnt(pawn & ours) - popcnt(pawn & theirs)) +
           300 * (popcnt(knight & ours) - popcnt(knight & theirs)) +
           350 * (popcnt(bishop & ours) - popcnt(bishop & theirs)) +
           500 * (popcnt(rook & ours) - popcnt(rook & theirs)) +
           900 * (popcnt(queen & ours) - popcnt(queen & theirs)) +
           1000 * (popcnt(king & ours) - popcnt(king & theirs));
}
