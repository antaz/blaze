#include "gen.h"
#include "attack.h"
#include "bitboard.h"
#include "pawn.h"
#include "type.h"

#include <stdio.h>

uint64_t danger(const uint64_t bb[], const int color)
{
        const uint64_t ours = bb[color];
        const uint64_t theirs = bb[!color];
        const uint64_t all = ours | theirs;

        uint64_t pawn, knight, bq, rq;
        uint64_t danger = 0ULL;

        pawn = bb[PAWN] & theirs;
        danger |= patks(pawn, color);

        danger |= katk(bsf(bb[KING] & theirs));

        knight = bb[KNIGHT] & theirs;
        while (knight) {
                danger |= natk(bsf(knight));
                knight &= knight - 1;
        }

        bq = (bb[BISHOP] | bb[QUEEN]) & theirs;
        while (bq) {
                danger |= batk(bsf(bq), all ^ (bb[KING] & ours));
                bq &= bq - 1;
        }

        rq = (bb[ROOK] | bb[QUEEN]) & theirs;
        while (rq) {
                danger |= ratk(bsf(rq), all ^ (bb[KING] & ours));
                rq &= rq - 1;
        }

        return danger;
}

uint64_t check(const uint64_t bb[], const int color)
{
        const int k = bsf(bb[KING] & bb[color]);
        const uint64_t all = bb[WHITE] | bb[BLACK];

        uint64_t pawn, knight, rq, bq;

        pawn = bb[PAWN] & bb[!color];
        knight = bb[KNIGHT] & bb[!color];
        rq = (bb[ROOK] | bb[QUEEN]) & bb[!color];
        bq = (bb[BISHOP] | bb[QUEEN]) & bb[!color];
        return (natk(k) & knight) | (batk(k, all) & bq) | (ratk(k, all) & rq) |
               (patks(bb[KING] & bb[color], color) & pawn);
}

uint64_t pin(const uint64_t bb[], const int color)
{
        const int k = bsf(bb[KING] & bb[color]);

        uint64_t pinned = 0ULL;
        uint64_t pinner;

        pinner = ratk(k, bb[!color]) & ((bb[ROOK] | bb[QUEEN]) & bb[!color]);
        pinner |= batk(k, bb[!color]) & ((bb[BISHOP] | bb[QUEEN]) & bb[!color]);
        while (pinner) {
                uint64_t p = between(bsf(pinner), k) & bb[color];
                if ((p & p - 1) == 0)
                        pinned |= p;
                pinner &= pinner - 1;
        }

        return pinned;
}

uint64_t atkfr(const uint64_t bb[], const int i, const int color)
{
        const uint64_t ours = bb[color];
        const uint64_t all = bb[color] | bb[!color];

        return (patk(i, color) & (bb[PAWN] & ours)) |
               (natk(i) & (bb[KNIGHT] & ours)) |
               (batk(i, all) & ((bb[BISHOP] | bb[QUEEN]) & ours)) |
               (ratk(i, all) & ((bb[ROOK] | bb[QUEEN]) & ours)) |
               (katk(i) & (bb[KING] & ours));
}

// generate white pawn moves
void genwp(const uint64_t bb[], uint16_t *moves)
{
        uint64_t ours = bb[WHITE];
        uint64_t theirs = bb[BLACK];
        uint64_t all = ours | theirs;
        uint64_t pawn = bb[PAWN] & ours;

        // single push
        uint64_t p = ((pawn & ~R7) << 8) & ~all;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to - 8, to, QUIET);
                p &= p - 1;
        }

        // double push
        p = (pawn << 8 & ~all) << 8 & R4 & ~all;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to - 16, to, DPP);
                p &= p - 1;
        }

        // captures
        p = ((pawn & ~R7) << 7) & theirs;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to - 7, to, CAPTURE);
                p &= p - 1;
        }

        p = ((pawn & ~R7) << 9) & theirs;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to - 9, to, CAPTURE);
                p &= p - 1;
        }

        // quiet promotion
        p = (pawn & R7) << 8 & ~all;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to - 8, to, NP);
                *moves++ = move(to - 8, to, BP);
                *moves++ = move(to - 8, to, RP);
                *moves++ = move(to - 8, to, QP);
                p &= p - 1;
        }

        // capture promotion
        p = ((pawn & R7) << 7) & theirs;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to - 7, to, NP);
                *moves++ = move(to - 7, to, BP);
                *moves++ = move(to - 7, to, RP);
                *moves++ = move(to - 7, to, QP);
                p &= p - 1;
        }

        p = ((pawn & R7) << 9) & theirs;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to - 9, to, NP);
                *moves++ = move(to - 9, to, BP);
                *moves++ = move(to - 9, to, RP);
                *moves++ = move(to - 9, to, QP);
                p &= p - 1;
        }
}

// generate black pawn moves
void genbp(const uint64_t *bb, uint16_t *moves)
{
        uint64_t ours = bb[BLACK];
        uint64_t theirs = bb[WHITE];
        uint64_t all = ours | theirs;
        uint64_t pawn = bb[PAWN] & ours;

        // single push
        uint64_t p = (pawn >> 8) & ~all;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to + 8, to, QUIET);
                p &= p - 1;
        }

        // double push
        p = (pawn >> 8 & ~all) >> 8 & R5 & ~all;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to + 16, to, DPP);
                p &= p - 1;
        }

        // captures
        uint64_t b = pawn & ~R2;
        while (b) {
                int from = bsf(b);
                p = patk(from, BLACK) & theirs;

                while (p) {
                        int to = bsf(p);
                        *moves++ = move(from, to, CAPTURE);
                        p &= p - 1;
                };

                b &= b - 1;
        }

        // quiet promotion
        p = (pawn & R2) >> 8 & ~all;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to + 8, to, NP);
                *moves++ = move(to + 8, to, BP);
                *moves++ = move(to + 8, to, RP);
                *moves++ = move(to + 8, to, QP);
                p &= p - 1;
        }

        // capture promotion
        p = ((pawn & R2) >> 7) & theirs;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to + 7, to, NP);
                *moves++ = move(to + 7, to, BP);
                *moves++ = move(to + 7, to, RP);
                *moves++ = move(to + 7, to, QP);
                p &= p - 1;
        }

        p = ((pawn & R2) >> 9) & theirs;
        while (p) {
                int to = bsf(p);
                *moves++ = move(to + 9, to, NP);
                *moves++ = move(to + 9, to, BP);
                *moves++ = move(to + 9, to, RP);
                *moves++ = move(to + 9, to, QP);
                p &= p - 1;
        }
}

void generate(const struct board_t *board, uint16_t *moves)
{
        const uint8_t turn = board->turn;
        const uint8_t ply = board->ply;
        const uint8_t ca = board->ca;
        const uint64_t *bb = board->bb;
        const uint8_t *mail = board->mail;
        const uint64_t ours = bb[turn];
        const uint64_t theirs = bb[!turn];
        const uint64_t all = ours | theirs;
        const int k = bsf(bb[KING] & ours);
        const uint64_t checker = check(bb, turn);
        const uint64_t pinned = pin(bb, turn);
        const struct undo_t *hist = board->hist;

        uint64_t target;
        uint64_t b;
        uint64_t capture;
        uint64_t quiet;

        // king moves
        target = katk(k) & ~(ours | danger(bb, turn));
        while (target) {
                *moves++ = move(k, bsf(target), 0);
                target &= target - 1;
        };

        switch (popcnt(checker)) {
        case 2:
                // double check
                return;
        case 1:
                switch (mail[bsf(checker)] & 7) {
                case PAWN:
                        // if(checker == push)) {
                        // 	target =
                        // }
                case KNIGHT:
                        target = atkfr(bb, bsf(checker), turn);
                        while (target) {
                                *moves++ =
                                    move(bsf(target), bsf(checker), CAPTURE);
                                target &= target - 1;
                        }
                        break;
                default:
                        capture = checker;
                        quiet = between(k, checker);
                        break;
                }
                break;

        default:
                // capture enemy pieces
                capture = theirs;

                // or play a quiet move to any square
                quiet = ~all;

                if (hist[ply].ep != NOSQ) {
                        target = patk(hist[ply].ep, turn) & (bb[PAWN] & ours);
                        while (target & ~pinned) {
                                b = bsf(target);
                                target &= target - 1;
                        }

                        target = patk(hist[ply].ep, turn) & (bb[PAWN] & ours);
                        b = target & pinned & line(hist[ply].ep, k);
                        if (b)
                                *moves++ = move(bsf(b), hist[ply].ep, EP);
                }

                // castling moves
                if (turn) {
                        // black to move
                } else {
                        // white to move
                }

                break;
        }

        // TODO: Generate pawn moves

        // Generate knight moves
        uint64_t knight = bb[KNIGHT] & ours;

        while (knight) {
                int from = bsf(knight);
                target = natk(from) & ~ours;

                while (target) {
                        int to = bsf(target);
                        *moves++ = move(from, to, 0);
                        target &= target - 1;
                };

                knight &= knight - 1;
        };
        // Generate bishop & queen diagonal moves
        uint64_t sliderd = (bb[BISHOP] | bb[QUEEN]) & ours;

        while (sliderd) {
                int from = bsf(sliderd);
                target = batk(from, all) & ~ours;

                while (target) {
                        int to = bsf(target);
                        *moves++ = move(from, to, 0);
                        target &= target - 1;
                }

                sliderd &= sliderd - 1;
        }
        // Generate rook & queen orthogonal moves
        uint64_t slidero = (bb[ROOK] | bb[QUEEN]) & ours;

        while (slidero) {
                int from = bsf(slidero);
                target = ratk(from, all) & ~ours;

                while (target) {
                        int to = bsf(target);
                        *moves++ = move(from, to, 0);
                        target &= target - 1;
                }
                slidero &= slidero - 1;
        }
}
