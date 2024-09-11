#include "gen.h"
#include "attack.h"
#include "bitboard.h"
#include "move.h"
#include <assert.h>

int quiet(const struct board_t *board, struct move_t *moves)
{
	/* TODO: this section needs to be abstracted */
	uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
	uint64_t ours = board->bb[0];
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
	for (uint64_t k = king; k; k &= k - 1) {
		for (p = katk(bsf(k)) & ~all; p; p &= p - 1) {
			moves->data = MOVE(bsf(k), bsf(p), QUIET);
			moves++;
			count++;
		}
	}

	// knight moves
	for (uint64_t n = knight; n; n &= n - 1) {
		for (p = natk(bsf(n)) & ~all; p; p &= p - 1) {
			moves->data = MOVE(bsf(n), bsf(p), QUIET);
			moves++;
			count++;
		}
	}

	// bishop & queen diagonal moves
	for (uint64_t d = (bishop | queen); d; d &= d - 1) {
		for (p = batk(bsf(d), all) & ~all; p; p &= p - 1) {
			moves->data = MOVE(bsf(d), bsf(p), QUIET);
			moves++;
			count++;
		}
	}

	// rook & queen orthogonal moves
	for (uint64_t o = (rook | queen); o; o &= o - 1) {
		for (p = ratk(bsf(o), all) & ~all; p; p &= p - 1) {
			moves->data = MOVE(bsf(o), bsf(p), QUIET);
			moves++;
			count++;
		}
	}

	// pawn push
	for (p = ((pawn & ~R7) << 8) & ~all; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 8, to, QUIET);
		moves++;
		count++;
	}

	// double pawn push
	for (p = (pawn << 8 & ~all) << 8 & R4 & ~all; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 16, to, DPP);
		moves++;
		count++;
	}

	// castling
	// TODO: refactor this
	uint8_t ca = board->ca;
	if ((ca & 0x02) && !(all & 0x60)) {
		uint64_t pawn =
		    (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & theirs;
		uint64_t knight =
		    (~board->bb[1] & board->bb[2] & ~board->bb[3]) & theirs;
		uint64_t bishop = (board->bb[1] & board->bb[2]) & theirs;
		uint64_t rook =
		    (~board->bb[1] & ~board->bb[2] & board->bb[3]) & theirs;
		uint64_t queen = (board->bb[1] & board->bb[3]) & theirs;
		uint64_t king = (board->bb[2] & board->bb[3]) & theirs;

		uint64_t orth = LSB(0x1010101010101000ULL & all);
		orth |= LSB(0x2020202020202000ULL & all);
		orth |= LSB(0x4040404040404000ULL & all);
		orth |= 1ULL << MSB(0x000000000000000FULL & (all | 0x1ULL));

		uint64_t diag = LSB(0x0000000102040800ULL & all);
		diag |= LSB(0x0000010204081000ULL & all);
		diag |= LSB(0x0001020408102000ULL & all);
		diag |= LSB(0x0000000080402000ULL & all);
		diag |= LSB(0x0000000000804000ULL & all);
		diag |= 0x0000000000008000ULL;

		if (!((orth & (rook | queen)) | (diag & (bishop | queen)) |
		      (0x0000000000F8DC00ULL & knight) |
		      (0x000000000000F800ULL & pawn) |
		      (0x0000000000004000ULL & king))) {
			moves->data = MOVE(4, 6, OO);
			moves->score = 0;
			moves++;
			count++;
		}
	}
	if ((ca & 0x01) && !(all & 0x0e)) {
		uint64_t pawn =
		    (board->bb[1] & ~board->bb[2] & ~board->bb[3]) & theirs;
		uint64_t knight =
		    (~board->bb[1] & board->bb[2] & ~board->bb[3]) & theirs;
		uint64_t bishop = (board->bb[1] & board->bb[2]) & theirs;
		uint64_t rook =
		    (~board->bb[1] & ~board->bb[2] & board->bb[3]) & theirs;
		uint64_t queen = (board->bb[1] & board->bb[3]) & theirs;
		uint64_t king = (board->bb[2] & board->bb[3]) & theirs;

		uint64_t orth = LSB(0x1010101010101000ULL & all);
		orth |= LSB(0x0808080808080800ULL & all);
		orth |= LSB(0x0404040404040400ULL & all);
		orth |= LSB(0x00000000000000E0ULL & all);

		uint64_t diag = LSB(0x0000000102040800ULL & all);
		diag |= LSB(0x0000000001020400ULL & all);
		diag |= LSB(0x0000000000010200ULL & all);
		diag |= LSB(0x0000000080402000ULL & all);
		diag |= LSB(0x0000008040201000ULL & all);
		diag |= LSB(0x0000804020100800ULL & all);

		if (!((orth & (rook | queen)) | (diag & (bishop | queen)) |
		      (0x00000000003E7700ULL & knight) |
		      (0x0000000000003E00ULL & pawn) |
		      (0x0000000000000600ULL & king))) {
			moves->data = MOVE(4, 2, OOO);
			moves++;
			count++;
		}
	}
	return count;
}

int noisy(const struct board_t *board, struct move_t *moves)
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
	for (uint64_t k = king; k; k &= k - 1) {
		for (p = katk(bsf(k)) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(k), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	// knight moves
	for (uint64_t n = knight; n; n &= n - 1) {
		for (p = natk(bsf(n)) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(n), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	// Generate bishop & queen diagonal moves
	for (uint64_t d = (bishop | queen); d; d &= d - 1) {
		for (p = batk(bsf(d), all) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(d), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	// Generate rook & queen orthogonal moves
	for (uint64_t o = (rook | queen); o; o &= o - 1) {
		for (p = ratk(bsf(o), all) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(o), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	// pawn captures
	for (p = ((pawn & ~R7) << 7) & ~FH & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 7, to, CAPTURE);
		moves++;
		count++;
	}

	for (p = ((pawn & ~R7) << 9) & ~FA & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 9, to, CAPTURE);
		moves++;
		count++;
	}

	// capture promotion
	for (p = ((pawn & R7) << 7) & ~FH & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 7, to, NPC);
		moves++;
		moves->data = MOVE(to - 7, to, BPC);
		moves++;
		moves->data = MOVE(to - 7, to, RPC);
		moves++;
		moves->data = MOVE(to - 7, to, QPC);
		moves++;
		count = count + 4;
	}

	for (p = ((pawn & R7) << 9) & ~FA & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 9, to, NPC);
		moves++;
		moves->data = MOVE(to - 9, to, BPC);
		moves++;
		moves->data = MOVE(to - 9, to, RPC);
		moves++;
		moves->data = MOVE(to - 9, to, QPC);
		moves++;
		count = count + 4;
	}

	// non-capturing promotion
	for (p = (pawn & R7) << 8 & ~all; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 8, to, NP);
		moves++;
		moves->data = MOVE(to - 8, to, BP);
		moves++;
		moves->data = MOVE(to - 8, to, RP);
		moves++;
		moves->data = MOVE(to - 8, to, QP);
		moves++;
		count = count + 4;
	}

	// en passant
	uint8_t ep = board->ep;
	if (ep != NOSQ) {
		// WAT! simplify this or find a better way
		for (p = pawn & (((1ULL << (ep + 24)) << 7 & ~FH) |
				 ((1ULL << (ep + 24)) << 9 & ~FA));
		     p; p &= p - 1) {
			moves->data = MOVE(bsf(p), 40 + ep, EP);
			moves++;
			count++;
		}
	}
	return count;
}

int capture(const struct board_t *board, struct move_t *moves)
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

	// capture promotion
	for (p = ((pawn & R7) << 7) & ~FH & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 7, to, NPC);
		moves++;
		moves->data = MOVE(to - 7, to, BPC);
		moves++;
		moves->data = MOVE(to - 7, to, RPC);
		moves++;
		moves->data = MOVE(to - 7, to, QPC);
		moves++;
		count = count + 4;
	}

	for (p = ((pawn & R7) << 9) & ~FA & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 9, to, NPC);
		moves++;
		moves->data = MOVE(to - 9, to, BPC);
		moves++;
		moves->data = MOVE(to - 9, to, RPC);
		moves++;
		moves->data = MOVE(to - 9, to, QPC);
		moves++;
		count = count + 4;
	}

	// pawn captures
	for (p = ((pawn & ~R7) << 7) & ~FH & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 7, to, CAPTURE);
		moves++;
		count++;
	}

	for (p = ((pawn & ~R7) << 9) & ~FA & theirs; p; p &= p - 1) {
		int to = bsf(p);
		moves->data = MOVE(to - 9, to, CAPTURE);
		moves++;
		count++;
	}

	// en passant
	uint8_t ep = board->ep;
	if (ep != NOSQ) {
		// WAT! simplify this or find a better way
		for (p = pawn & (((1ULL << (ep + 24)) << 7 & ~FH) |
				 ((1ULL << (ep + 24)) << 9 & ~FA));
		     p; p &= p - 1) {
			moves->data = MOVE(bsf(p), 40 + ep, EP);
			moves++;
			count++;
		}
	}

	// Generate bishop & queen diagonal moves
	for (uint64_t d = (bishop | queen); d; d &= d - 1) {
		for (p = batk(bsf(d), all) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(d), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	// Generate rook & queen orthogonal moves
	for (uint64_t o = (rook | queen); o; o &= o - 1) {
		for (p = ratk(bsf(o), all) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(o), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	// king moves
	for (uint64_t k = king; k; k &= k - 1) {
		for (p = katk(bsf(k)) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(k), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	// knight moves
	for (uint64_t n = knight; n; n &= n - 1) {
		for (p = natk(bsf(n)) & theirs; p; p &= p - 1) {
			moves->data = MOVE(bsf(n), bsf(p), CAPTURE);
			moves++;
			count++;
		}
	}

	return count;
}

uint64_t legal(const struct board_t *board, uint16_t move)
{
	int from, to;
	uint8_t ksq;
	from = MOVE_FROM(move);
	to = MOVE_TO(move);
	uint64_t frombb = 1ULL << from;
	uint64_t tobb = 1ULL << to;

	uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
	uint64_t theirs = all ^ board->bb[0];
	uint64_t pawn = (board->bb[1] & ~board->bb[2] & ~board->bb[3]);
	uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]);
	uint64_t bishop = (board->bb[1] & board->bb[2]);
	uint64_t rook = (~board->bb[1] & ~board->bb[2] & board->bb[3]);
	uint64_t queen = (board->bb[1] & board->bb[3]);
	uint64_t king = (board->bb[2] & board->bb[3]);

	// update all and theirs
	all = (all ^ frombb) | tobb;
	theirs = theirs & ~tobb;

	// TODO: redundant mailbox for faster piece lookup?
	uint8_t piece = ((board->bb[3] >> (from)) & 1) << 2 |
			((board->bb[2] >> (from)) & 1) << 1 |
			((board->bb[1] >> (from)) & 1);

	if (piece == KING) {
		king = tobb;
		ksq = MOVE_TO(move);
	} else {
		king = king & board->bb[0];
		assert(king);
		ksq = bsf(king);
		if (((move >> 12) & 0x0f) == EP) {
			theirs ^= tobb >> 8;
			all ^= tobb >> 8;
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

uint64_t check(const struct board_t *board)
{
	uint64_t all = board->bb[1] | board->bb[2] | board->bb[3];
	uint64_t theirs = all ^ board->bb[0];
	uint64_t pawn = (board->bb[1] & ~board->bb[2] & ~board->bb[3]);
	uint64_t knight = (~board->bb[1] & board->bb[2] & ~board->bb[3]);
	uint64_t bishop = (board->bb[1] & board->bb[2]);
	uint64_t rook = (~board->bb[1] & ~board->bb[2] & board->bb[3]);
	uint64_t queen = (board->bb[1] & board->bb[3]);
	uint64_t king = (board->bb[2] & board->bb[3]) & board->bb[0];

	uint64_t ksq = bsf(king);

	return (((natk(ksq) & knight) | (ratk(ksq, all) & (rook | queen)) |
		 (batk(ksq, all) & (bishop | queen)) |
		 ((((king << 9) & 0xFEFEFEFEFEFEFEFEULL) |
		   ((king << 7) & 0x7F7F7F7F7F7F7F7FULL)) &
		  pawn) |
		 (katk(ksq) & (board->bb[2] & board->bb[3]))) &
		theirs);
}

int gen(const struct board_t *board, struct move_t *moves)
{
	// generating pseudo-legal moves
	int count = 0;
	count = noisy(board, moves);
	count += quiet(board, moves + count);
	return count;
}

int gen_legal(const struct board_t *board, struct move_t *moves)
{
	// generating legal moves
	struct move_t m[320] = {{.data = 0, .score = 0}};
	int c = gen(board, m);
	int count = 0;
	for (int i = 0; i < c; i++) {
		if (legal(board, m[i].data)) {
			continue;
		}

		*moves++ = m[i];
		count++;
	}

	return count;
}
