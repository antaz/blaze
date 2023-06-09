#include "gen.h"
#include "attack.h"
#include "bitboard.h"
#include "type.h"

#include <stdio.h>

void quiet(const struct board_t *board, uint16_t *moves)
{
        uint64_t ours   = board->bb[0];
        uint64_t all    = board->bb[1] | board->bb[2] | board->bb[3];
        uint64_t pawn   = (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & ours;
	uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]) & ours;
	uint64_t bishop = (board->bb[1] & board->bb[2]) & ours;
	uint64_t rook   = (~board->bb[1] & ~board->bb[2] & board->bb[3]) & ours;
	uint64_t queen  = (board->bb[1] & board->bb[3]) & ours;
	uint64_t king   = (board->bb[2] & board->bb[3]) & ours;
        uint64_t p;

	// king moves
        for(int i = bsf(king), p = katk(i) & ~all; p; p &= p - 1) {
		*moves++ = move(i, bsf(p), 0);
	}

	// knight moves
	for(uint64_t n = knight; n; n &= n - 1) {
		for(p = natk(bsf(n)) & ~all; p; p &= p - 1) {
			*moves++ = move(bsf(n), bsf(p), 0);
		}
	}

        // Generate bishop & queen diagonal moves
	for(uint64_t d = (bishop | queen); d; d &= d - 1) {
		for(p = batk(bsf(d), all) & ~all; p; p &= p - 1) {
			*moves++ = move(bsf(d), bsf(p), 0);
		}
	}

        // Generate rook & queen orthogonal moves
	for(uint64_t o = (rook | queen); o; o &= o - 1) {
		for(p = ratk(bsf(o), all) & ~all; p; p &= p - 1) {
			*moves++ = move(bsf(o), bsf(p), 0);
		}
	}

        // pawn push
	for(p = ((pawn & ~R7) << 8) & ~all; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = move(to - 8, to, QUIET);
	}

        // double pawn push
        for(p = (pawn << 8 & ~all) << 8 & R4 & ~all; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = move(to - 16, to, DPP);
        }

	// TODO: castling moves
}

void noisy(const struct board_t *board, uint16_t *moves)
{
        uint64_t ours   = board->bb[0];
        uint64_t all    = board->bb[1] | board->bb[2] | board->bb[3];
        uint64_t theirs = all ^ ours;
        uint64_t pawn   = (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & ours;
	uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]) & ours;
	uint64_t bishop = (board->bb[1] & board->bb[2]) & ours;
	uint64_t rook   = (~board->bb[1] & ~board->bb[2] & board->bb[3]) & ours;
	uint64_t queen  = (board->bb[1] & board->bb[3]) & ours;
	uint64_t king   = (board->bb[2] & board->bb[3]) & ours;
        uint64_t p;

	// king moves
        for(int i = bsf(king), p = katk(i) & theirs; p; p &= p - 1) {
		*moves++ = move(i, bsf(p), 0);
	}

	// knight moves
	for(uint64_t n = knight; n; n &= n - 1) {
		for(p = natk(bsf(n)) & theirs; p; p &= p - 1) {
			*moves++ = move(bsf(n), bsf(p), 0);
		}
	}

        // Generate bishop & queen diagonal moves
	for(uint64_t d = (bishop | queen); d; d &= d - 1) {
		for(p = batk(bsf(d), all) & theirs; p; p &= p - 1) {
			*moves++ = move(bsf(d), bsf(p), 0);
		}
	}

        // Generate rook & queen orthogonal moves
	for(uint64_t o = (rook | queen); o; o &= o - 1) {
		for(p = ratk(bsf(o), all) & theirs; p; p &= p - 1) {
			*moves++ = move(bsf(o), bsf(p), 0);
		}
	}

	// pawn captures
        for(p = ((pawn & ~R7) << 7) & ~FH & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = move(to - 7, to, CAPTURE);
        }

        for(p = ((pawn & ~R7) << 9) & ~FA & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = move(to - 9, to, CAPTURE);
        }

        // capture promotion
        for(p = ((pawn & R7) << 7) & ~FH & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = move(to - 7, to, NP);
                *moves++ = move(to - 7, to, BP);
                *moves++ = move(to - 7, to, RP);
                *moves++ = move(to - 7, to, QP);
        }

	for(p = ((pawn & R7) << 9) & ~FA & theirs; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = move(to - 9, to, NP);
                *moves++ = move(to - 9, to, BP);
                *moves++ = move(to - 9, to, RP);
                *moves++ = move(to - 9, to, QP);
        }

	// non-capturing promotion
        for(p = (pawn & R7) << 8 & ~all; p; p &= p - 1) {
                int to = bsf(p);
                *moves++ = move(to - 8, to, NP);
                *moves++ = move(to - 8, to, BP);
                *moves++ = move(to - 8, to, RP);
                *moves++ = move(to - 8, to, QP);
        }

	// en passant
	uint8_t ep = board->ep;
	if(ep != NOSQ) {
		p = pawn & (((1ULL << ep) >> 7 & ~FA) | ((1ULL << ep) >> 9 & ~FH));
		*moves++ = move(bsf(p), ep, EP);
	}
}
