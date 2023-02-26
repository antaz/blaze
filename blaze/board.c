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

void generate(const struct board_t *board, uint16_t *moves)
{

        bool turn = board->turn;
        uint64_t ours = board->piece[turn];
        uint64_t theirs = board->piece[!turn];
        uint64_t all = ours | theirs;

        // Generate pawn moves
        if (turn) {
                uint64_t bpawn = board->piece[PAWN] & ours;

                // Single push
                uint64_t push = (bpawn >> 8) & ~all;

                while (push) {
                        int to = bsf(push);
                        *moves++ = move(to + 8, to, QUIET);
                        push &= push - 1;
                }

                // Double push
                uint64_t dpush =
                    ((bpawn << 8) & ~all) << 8 & 0x000000FF00000000 & ~all;

                while (dpush) {
                        int to = bsf(dpush);
                        *moves++ = move(to + 16, to, QUIET);
                        dpush &= dpush - 1;
                }

                // Capture
                while (bpawn) {
                        int from = bsf(bpawn);
                        uint64_t target = bpattack(from) & theirs;

                        while (target) {
                                int to = bsf(target);
                                *moves++ = move(from, to, CAPTURE);
                                target &= target - 1;
                        }

                        bpawn &= bpawn - 1;
                }
        } else {
                uint64_t wpawn = board->piece[PAWN] & ours;

                // Single push
                uint64_t push = (wpawn << 8) & ~all;

                while (push) {
                        int to = bsf(push);
                        *moves++ = move(to - 8, to, QUIET);
                        push &= push - 1;
                }

                // Double push
                uint64_t dpush =
                    ((wpawn << 8) & ~all) << 8 & 0x00000000FF000000 & ~all;

                while (dpush) {
                        int to = bsf(dpush);
                        *moves++ = move(to - 16, to, QUIET);
                        dpush &= dpush - 1;
                }

                // Capture
                while (wpawn) {
                        int from = bsf(wpawn);
                        uint64_t target = wpattack(from) & theirs;

                        while (target) {
                                int to = bsf(target);
                                *moves++ = move(from, to, CAPTURE);
                                target &= target - 1;
                        };

                        wpawn &= wpawn - 1;
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
        // Generate bishop moves
        // Generate rook moves
        // Generate queen moves
        // Generate king moves
        uint64_t king = board->piece[KING] & ours;

        while (king) {
                int from = bsf(king);
                uint64_t target = kattack(from) & ~ours;

                while (target) {
                        int to = bsf(target);
                        *moves++ = move(from, to, 0);
                        target &= target - 1;
                };

                king &= king - 1;
        };
}

void make(struct board_t *board, const uint16_t *move) {}

void unmake(struct board_t *board) {}
