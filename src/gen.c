#include "gen.h"
#include "attack.h"
#include "bitboard.h"
#include "type.h"
#include <stdint.h>

int quiet(const struct board_t *board, uint16_t *moves)
{
        /* TODO: this section needs to be abstracted */
        uint64_t ours = board->bb[0];
        uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
        uint64_t pawn = (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & ours;
        uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]) & ours;
        uint64_t bishop = (board->bb[1] & board->bb[2]) & ours;
        uint64_t rook = (~board->bb[1] & ~board->bb[2] & board->bb[3]) & ours;
        uint64_t queen = (board->bb[1] & board->bb[3]) & ours;
        uint64_t king = (board->bb[2] & board->bb[3]) & ours;
        uint64_t p;
        int count = 0;

        // king moves
        for (int i = bsf(king), p = katk(i) & ~all; p; p &= p - 1) {
                *moves++ = MOVE(i, bsf(p), QUIET);
                count++;
        }

        // knight moves
        for (uint64_t n = knight; n; n &= n - 1) {
                for (p = natk(bsf(n)) & ~all; p; p &= p - 1) {
                        *moves++ = MOVE(bsf(n), bsf(p), QUIET);
                        count++;
                }
        }

        // bishop & queen diagonal moves
        for (uint64_t d = (bishop | queen); d; d &= d - 1) {
                for (p = batk(bsf(d), all) & ~all; p; p &= p - 1) {
                        *moves++ = MOVE(bsf(d), bsf(p), QUIET);
                        count++;
                }
        }

        // rook & queen orthogonal moves
        for (uint64_t o = (rook | queen); o; o &= o - 1) {
                for (p = ratk(bsf(o), all) & ~all; p; p &= p - 1) {
                        *moves++ = MOVE(bsf(o), bsf(p), QUIET);
                        count++;
                }
        }

        // pawn push
        for (p = ((pawn & ~R7) << 8) & ~all; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = MOVE(to - 8, to, QUIET);
                count++;
        }

        // double pawn push
        for (p = (pawn << 8 & ~all) << 8 & R4 & ~all; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = MOVE(to - 16, to, DPP);
                count++;
        }

        // castling
        uint8_t ca = board->ca;
        if ((ca & 0x02) && !(all & 0x60)) {
                *moves++ = MOVE(4, 6, OO);
                count++;
        } else if ((ca & 0x01) && !(all & 0xe)) {
                *moves++ = MOVE(4, 2, OOO);
                count++;
        }
        return count;
}

int noisy(const struct board_t *board, uint16_t *moves)
{
        uint64_t ours = board->bb[0];
        uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
        uint64_t theirs = all ^ ours;
        uint64_t pawn = (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & ours;
        uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]) & ours;
        uint64_t bishop = (board->bb[1] & board->bb[2]) & ours;
        uint64_t rook = (~board->bb[1] & ~board->bb[2] & board->bb[3]) & ours;
        uint64_t queen = (board->bb[1] & board->bb[3]) & ours;
        uint64_t king = (board->bb[2] & board->bb[3]) & ours;
        uint64_t p;
        int count = 0;

        // king moves
        for (int i = bsf(king), p = katk(i) & theirs; p; p &= p - 1) {
                *moves++ = MOVE(i, bsf(p), CAPTURE);
                count++;
        }

        // knight moves
        for (uint64_t n = knight; n; n &= n - 1) {
                for (p = natk(bsf(n)) & theirs; p; p &= p - 1) {
                        *moves++ = MOVE(bsf(n), bsf(p), CAPTURE);
                        count++;
                }
        }

        // Generate bishop & queen diagonal moves
        for (uint64_t d = (bishop | queen); d; d &= d - 1) {
                for (p = batk(bsf(d), all) & theirs; p; p &= p - 1) {
                        *moves++ = MOVE(bsf(d), bsf(p), CAPTURE);
                        count++;
                }
        }

        // Generate rook & queen orthogonal moves
        for (uint64_t o = (rook | queen); o; o &= o - 1) {
                for (p = ratk(bsf(o), all) & theirs; p; p &= p - 1) {
                        *moves++ = MOVE(bsf(o), bsf(p), CAPTURE);
                        count++;
                }
        }

        // pawn captures
        for (p = ((pawn & ~R7) << 7) & ~FH & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = MOVE(to - 7, to, CAPTURE);
                count++;
        }

        for (p = ((pawn & ~R7) << 9) & ~FA & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = MOVE(to - 9, to, CAPTURE);
                count++;
        }

        // capture promotion
        for (p = ((pawn & R7) << 7) & ~FH & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = MOVE(to - 7, to, NP);
                *moves++ = MOVE(to - 7, to, BP);
                *moves++ = MOVE(to - 7, to, RP);
                *moves++ = MOVE(to - 7, to, QP);
                count = count + 4;
        }

        for (p = ((pawn & R7) << 9) & ~FA & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = MOVE(to - 9, to, NP);
                *moves++ = MOVE(to - 9, to, BP);
                *moves++ = MOVE(to - 9, to, RP);
                *moves++ = MOVE(to - 9, to, QP);
                count = count + 4;
        }

        // non-capturing promotion
        for (p = (pawn & R7) << 8 & ~all; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = MOVE(to - 8, to, NP);
                *moves++ = MOVE(to - 8, to, BP);
                *moves++ = MOVE(to - 8, to, RP);
                *moves++ = MOVE(to - 8, to, QP);
                count = count + 4;
        }

        // en passant
        uint8_t ep = board->ep;
        if (ep != NOSQ) {
                p = pawn &
                    (((1ULL << ep) >> 7 & ~FA) | ((1ULL << ep) >> 9 & ~FH));
                *moves++ = MOVE(bsf(p), ep, EP);
                count++;
        }
        return count;
}

uint64_t legal(const struct board_t *board, uint16_t move)
{
        uint8_t ksq;
        uint64_t from = 1ULL << MOVE_FROM(move);
        uint64_t to = 1ULL << MOVE_TO(move);

        uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
        uint64_t theirs = all ^ board->bb[0];
        uint64_t pawn = (board->bb[1] & ~board->bb[2] & ~board->bb[3]);
        uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]);
        uint64_t bishop = (board->bb[1] & board->bb[2]);
        uint64_t rook = (~board->bb[1] & ~board->bb[2] & board->bb[3]);
        uint64_t queen = (board->bb[1] & board->bb[3]);
        uint64_t king = (board->bb[2] & board->bb[3]);

        // update all and theirs
        all = (all ^ from) | to;
        theirs = theirs & ~to;
        uint8_t piece = ((board->bb[3] >> (MOVE_FROM(move))) & 1) << 2 |
                        ((board->bb[2] >> (MOVE_FROM(move))) & 1) << 1 |
                        ((board->bb[1] >> (MOVE_FROM(move))) & 1);

        if (piece == KING) {
                king = to;
                ksq = MOVE_TO(move);
        } else {
                ksq = bsf(king & board->bb[0]);
                if (((move >> 12) & 0x0f) == EP) {
                        theirs ^= to >> 8;
                        all ^= to >> 8;
                }
        }
        return (((natk(ksq) & knight) | (ratk(ksq, all) & (rook | queen)) |
                 (batk(ksq, all) & (bishop | queen)) |
                 ((((king << 9) & 0xFEFEFEFEFEFEFEFEULL) |
                   ((king << 7) & 0x7F7F7F7F7F7F7F7FULL)) &
                  pawn) |
                 (katk(ksq) & (board->bb[2] & board->bb[3]))) &
                theirs);
}
