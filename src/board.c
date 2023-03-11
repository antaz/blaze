#include "board.h"
#include "bitboard.h"
#include "move.h"
#include <stdio.h>

void parse(const char *fen, struct board_t *board)
{
        int i = 0;
        while (*fen) {
                switch (*fen) {
                case 'p':
                        set(board->piece[0], i);
                        set(board->piece[2], i++);
                        break;
                case 'n':
                        set(board->piece[0], i);
                        set(board->piece[3], i++);
                        break;
                case 'b':
                        set(board->piece[0], i);
                        set(board->piece[4], i++);
                        break;
                case 'r':
                        set(board->piece[0], i);
                        set(board->piece[5], i++);
                        break;
                case 'q':
                        set(board->piece[0], i);
                        set(board->piece[6], i++);
                        break;
                case 'k':
                        set(board->piece[0], i);
                        set(board->piece[7], i++);
                        break;
                case 'P':
                        set(board->piece[1], i);
                        set(board->piece[2], i++);
                        break;
                case 'N':
                        set(board->piece[1], i);
                        set(board->piece[3], i++);
                        break;
                case 'B':
                        set(board->piece[1], i);
                        set(board->piece[4], i++);
                        break;
                case 'R':
                        set(board->piece[1], i);
                        set(board->piece[5], i++);
                        break;
                case 'Q':
                        set(board->piece[1], i);
                        set(board->piece[6], i++);
                        break;
                case 'K':
                        set(board->piece[1], i);
                        set(board->piece[7], i++);
                        break;

                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                        i += *fen - '0';
                        break;

                case '/':
                case ' ':
                        ++fen;
                        continue;

                default:
                        printf("no match!\n");
                }
                ++fen;
        }
}

/* chekers set */
uint64_t checker(const uint64_t piece[], bool color)
{
        uint64_t pawn, knight, rq, bq;
        uint64_t all = piece[WHITE] | piece[BLACK];
        int ksq = bsf(piece[KING] & piece[color]);

        uint64_t checker = 0ULL;

        pawn = piece[PAWN] & piece[!color];
        knight = piece[KNIGHT] & piece[!color];
        rq = (piece[ROOK] | piece[QUEEN]) & piece[!color];
        bq = (piece[BISHOP] | piece[QUEEN]) & piece[!color];
        checker = (nattack(ksq) & knight) | (battack(ksq, all) & bq) | (rattack(ksq, all) & rq);

        if(color)
                checker |= ((pawn << 7) & 0x7f7f7f7f7f7f7f7f) | ((pawn << 9) & 0xfefefefefefefefe);
        else
                checker |= ((pawn >> 7) & 0xfefefefefefefefe) | ((pawn >> 9) & 0x7f7f7f7f7f7f7f7f);
        
        return checker &= pawn;
}

/* squares the king cannot move to */
uint64_t danger(const uint64_t piece[], bool color)
{
        uint64_t pawn, knight, bq, rq, king;
        uint64_t ours = piece[color];
        uint64_t theirs = piece[!color];
        uint64_t all = ours | theirs;

        uint64_t danger = 0ULL;

        pawn = piece[PAWN] & theirs;
        if(color)
                danger |= ((pawn << 7) & 0x7f7f7f7f7f7f7f7f) | ((pawn << 9) & 0xfefefefefefefefe);
        else
                danger |= ((pawn >> 7) & 0xfefefefefefefefe) | ((pawn >> 9) & 0x7f7f7f7f7f7f7f7f);

        danger |= kattack(bsf(piece[KING] & theirs));

        knight = piece[KNIGHT] & theirs;
        while (knight) {
                danger |= nattack(bsf(knight));
                knight &= knight - 1;
        }

        bq = (piece[BISHOP] | piece[QUEEN]) & theirs;
        while (bq) {
                danger |= battack(bsf(bq), all ^ (piece[KING] & ours));
                bq &= bq - 1;
        }

        rq = (piece[ROOK] | piece[QUEEN]) & theirs;
        while (rq) {
                danger |= rattack(bsf(rq), all ^ (piece[KING] & ours));
                rq &= rq - 1;
        }

        return danger;
}

uint64_t pinned(const uint64_t *piece[], bool color)
{
}

void generate(const struct board_t *board, uint16_t *moves)
{

        bool turn = board->turn;
        uint64_t ours = board->piece[turn];
        uint64_t theirs = board->piece[!turn];
        uint64_t all = ours | theirs;
        int ksq = bsf(board->piece[KING] & ours);
        uint64_t checker;
        uint64_t pinned = 0ULL;

        // king moves
        uint64_t target = kattack(ksq) & ~(ours | danger(board->piece, turn));
        while (target) {
                *moves++ = move(ksq, bsf(target), 0);
                target &= target - 1;
        };

        switch (popcount(checker)) {
        case 2:
                return;
        case 1:
                break;
        default:
                break;
        }

        // Generate pawn moves
        if (turn) {
                uint64_t pawn = board->piece[PAWN] & ours;

                // Single push
                uint64_t push = (pawn >> 8) & ~all;

                while (push) {
                        int to = bsf(push);
                        *moves++ = move(to + 8, to, QUIET);
                        push &= push - 1;
                }

                // Double push
                uint64_t dpush =
                    ((pawn >> 8) & ~all) >> 8 & 0x000000FF00000000 & ~all;

                while (dpush) {
                        int to = bsf(dpush);
                        *moves++ = move(to + 16, to, DPP);
                        dpush &= dpush - 1;
                }

                // Capture
                // black pawns not in the 2nd rank
                uint64_t b = pawn & ~0x000000000000FF00;
                while (b) {
                        int from = bsf(b);
                        uint64_t target = pattack(from, turn) & theirs;

                        while (target) {
                                int to = bsf(target);
                                *moves++ = move(from, to, CAPTURE);
                                target &= target - 1;
                        }

                        b &= b - 1;
                }

                // Quiet promotion
                b = ((pawn & 0x000000000000FF00) >> 8) & ~all;
                while (b) {
                        int to = bsf(b);
                        *moves++ = move(to + 8, to, NP);
                        *moves++ = move(to + 8, to, BP);
                        *moves++ = move(to + 8, to, RP);
                        *moves++ = move(to + 8, to, QP);
                        b &= b - 1;
                }

                // Capture promotion
                b = ((pawn & 0x000000000000FF00) >> 7) & theirs;
                while (b) {
                        int to = bsf(b);
                        *moves++ = move(to + 7, to, NP);
                        *moves++ = move(to + 7, to, BP);
                        *moves++ = move(to + 7, to, RP);
                        *moves++ = move(to + 7, to, QP);
                        b &= b - 1;
                }

                b = ((pawn & 0x000000000000FF00) >> 9) & theirs;
                while (b) {
                        int to = bsf(b);
                        *moves++ = move(to + 9, to, NP);
                        *moves++ = move(to + 9, to, BP);
                        *moves++ = move(to + 9, to, RP);
                        *moves++ = move(to + 9, to, QP);
                        b &= b - 1;
                }
        } else {
                uint64_t pawn = board->piece[PAWN] & theirs;

                // Single push
                uint64_t push = (pawn << 8) & ~all;
                while (push) {
                        int to = bsf(push);
                        *moves++ = move(to - 8, to, QUIET);
                        push &= push - 1;
                }

                // Double push
                uint64_t dpush =
                    ((pawn << 8) & ~all) << 8 & 0x00000000FF000000 & ~all;
                while (dpush) {
                        int to = bsf(dpush);
                        *moves++ = move(to - 16, to, DPP);
                        dpush &= dpush - 1;
                }

                // Capture
                // white pawns not in the 7th rank
                uint64_t b = pawn & ~0x00FF000000000000;
                while (b) {
                        int from = bsf(b);
                        uint64_t target = pattack(from, turn) & theirs;

                        while (target) {
                                int to = bsf(target);
                                *moves++ = move(from, to, CAPTURE);
                                target &= target - 1;
                        };

                        b &= b - 1;
                }


                // Quiet promotion
                b = ((pawn & 0x00FF000000000000) << 8) & ~all;
                while (b) {
                        int to = bsf(b);
                        *moves++ = move(to - 8, to, NP);
                        *moves++ = move(to - 8, to, BP);
                        *moves++ = move(to - 8, to, RP);
                        *moves++ = move(to - 8, to, QP);
                        b &= b - 1;
                }

                // Capture promotion
                b = ((pawn & 0x00FF000000000000) << 7) & theirs;
                while (b) {
                        int to = bsf(b);
                        *moves++ = move(to - 7, to, NP);
                        *moves++ = move(to - 7, to, BP);
                        *moves++ = move(to - 7, to, RP);
                        *moves++ = move(to - 7, to, QP);
                        b &= b - 1;
                }

                b = ((pawn & 0x00FF000000000000) << 9) & theirs;
                while (b) {
                        int to = bsf(b);
                        *moves++ = move(to - 9, to, NP);
                        *moves++ = move(to - 9, to, BP);
                        *moves++ = move(to - 9, to, RP);
                        *moves++ = move(to - 9, to, QP);
                        b &= b - 1;
                }
        }

        // Generate knight moves
        uint64_t knight = board->piece[KNIGHT] & ours;

        while (knight) {
                int from = bsf(knight);
                uint64_t target = nattack(from) & ~ours;

                while (target) {
                        int to = bsf(target);
                        *moves++ = move(from, to, 0);
                        target &= target - 1;
                };

                knight &= knight - 1;
        };
        // Generate bishop & queen diagonal moves
        uint64_t sliderd = (board->piece[BISHOP] | board->piece[QUEEN]) & ours;

        while (sliderd) {
                int from = bsf(sliderd);
                uint64_t target = battack(from, all) & ~ours;

                while (target) {
                        int to = bsf(target);
                        *moves++ = move(from, to, 0);
                        target &= target - 1;
                }

                sliderd &= sliderd - 1;
        }
        // Generate rook & queen orthogonal moves
        uint64_t slidero = (board->piece[ROOK] | board->piece[QUEEN]) & ours;

        while (slidero) {
                int from = bsf(slidero);
                uint64_t target = rattack(from, all) & ~ours;

                while (target) {
                        int to = bsf(target);
                        *moves++ = move(from, to, 0);
                        target &= target - 1;
                }
                slidero &= slidero - 1;
        }
}

void make(struct board_t *board, const uint16_t *move) {}

void unmake(struct board_t *board) {}
