#include "board.h"
#include "bitboard.h"
#include "type.h"

void parse(const char *fen, struct board_t *board)
{
        uint64_t *bb = board->bb;

        int i = 0;
        while (*fen != ' ') {
                switch (*fen) {
                case 'p':
                        bb[1] |= ((uint64_t)PAWN & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(PAWN >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)PAWN >> 2) << (i++ ^ 0x38);
                        break;
                case 'n':
                        bb[1] |= ((uint64_t)KNIGHT & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(KNIGHT >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)KNIGHT >> 2) << (i++ ^ 0x38);
                        break;
                case 'b':
                        bb[1] |= ((uint64_t)BISHOP & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(BISHOP >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)BISHOP >> 2) << (i++ ^ 0x38);
                        break;
                case 'r':
                        bb[1] |= ((uint64_t)ROOK & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(ROOK >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)ROOK >> 2) << (i++ ^ 0x38);
                        break;
                case 'q':
                        bb[1] |= ((uint64_t)QUEEN & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(QUEEN >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)QUEEN >> 2) << (i++ ^ 0x38);
                        break;
                case 'k':
                        bb[1] |= ((uint64_t)KING & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(KING >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)KING >> 2) << (i++ ^ 0x38);
                        break;
                case 'P':
                        bb[1] |= ((uint64_t)PAWN & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(PAWN >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)PAWN >> 2) << (i ^ 0x38);
                        bb[0] |= 1ULL << (i++ ^ 0x38);
                        break;
                case 'N':
                        bb[1] |= ((uint64_t)KNIGHT & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(KNIGHT >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)KNIGHT >> 2) << (i ^ 0x38);
                        bb[0] |= 1ULL << (i++ ^ 0x38);
                        break;
                case 'B':
                        bb[1] |= ((uint64_t)BISHOP & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(BISHOP >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)BISHOP >> 2) << (i ^ 0x38);
                        bb[0] |= 1ULL << (i++ ^ 0x38);
                        break;
                case 'R':
                        bb[1] |= ((uint64_t)ROOK & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(ROOK >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)ROOK >> 2) << (i ^ 0x38);
                        bb[0] |= 1ULL << (i++ ^ 0x38);
                        break;
                case 'Q':
                        bb[1] |= ((uint64_t)QUEEN & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(QUEEN >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)QUEEN >> 2) << (i ^ 0x38);
                        bb[0] |= 1ULL << (i++ ^ 0x38);
                        break;
                case 'K':
                        bb[1] |= ((uint64_t)KING & 1) << (i ^ 0x38);
                        bb[2] |= ((uint64_t)(KING >> 1) & 1) << (i ^ 0x38);
                        bb[3] |= ((uint64_t)KING >> 2) << (i ^ 0x38);
                        bb[0] |= 1ULL << (i++ ^ 0x38);
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
                        ++fen;
                        continue;

                default:
                        break;
                }
                ++fen;
        }

        ++fen;

        board->turn = *fen == 'w' ? WHITE : BLACK;

        fen += 2;

        if (*fen != '-') {
                for (; *fen != ' '; fen++) {
                        switch (*fen) {
                        case 'K':
                                board->ca |= WK;
                                break;
                        case 'Q':
                                board->ca |= WQ;
                                break;
                        case 'k':
                                board->ca |= BK;
                                break;
                        case 'q':
                                board->ca |= BQ;
                                break;
                        }
                }
                fen++;
        } else {
                fen += 2;
        }

        if (*fen != '-') {
                board->ep = 40 + (*fen - 'a');
        }

        if (board->turn == BLACK) {
                flip(board);
        }
}

uint64_t pawns(struct board_t *board)
{
        return board->bb[1] & ~board->bb[2] & ~board->bb[3];
}

uint64_t knights(struct board_t *board)
{
        return ~board->bb[1] & board->bb[2] & ~board->bb[3];
}

uint64_t bishops(struct board_t *board) { return board->bb[1] & board->bb[2]; }

uint64_t rooks(struct board_t *board)
{
        return ~board->bb[1] & ~board->bb[2] & board->bb[3];
}

uint64_t queens(struct board_t *board) { return board->bb[1] & board->bb[3]; }

uint64_t kings(struct board_t *board) { return board->bb[2] & board->bb[3]; }

uint64_t all(struct board_t *board)
{
        return board->bb[1] & board->bb[2] & board->bb[3];
}

void flip(struct board_t *board)
{
        uint64_t *bb = board->bb;

        bb[0] ^= bb[1] | bb[2] | bb[3];
        bb[0] = rev(bb[0]);
        bb[1] = rev(bb[1]);
        bb[2] = rev(bb[2]);
        bb[3] = rev(bb[3]);
        board->turn ^= BLACK;
        board->ca = (board->ca >> 4) | (board->ca << 4);
}

void make(struct board_t *board, const uint16_t move)
{
        uint64_t *bb, frombb, tobb;
        uint8_t from, to, piece;

        bb = board->bb;
        from = MOVE_FROM(move);
        to = MOVE_TO(move);
        frombb = 1ULL << ((move >> 6) & 0x3f);
        tobb = 1ULL << (move & 0x3f);

        switch (MOVE_TYPE(move)) {
        case CAPTURE:
                bb[1] &= ~tobb;
                bb[2] &= ~tobb;
                bb[3] &= ~tobb;

                if (to == 63)
                        board->ca &= 0xDF;
                else if (to == 56)
                        board->ca &= 0xEF;
        case QUIET:
                bb[0] ^= frombb | tobb;

                piece = ((bb[3] >> (from)) & 1) << 2 |
                        ((bb[2] >> (from)) & 1) << 1 | ((bb[1] >> (from)) & 1);

                switch (piece) {
                case PAWN:
                        bb[1] ^= frombb | tobb;
                        break;
                case KNIGHT:
                        bb[1] ^= 0;
                        bb[2] ^= frombb | tobb;
                        bb[3] ^= 0;
                        break;
                case BISHOP:
                        bb[1] ^= frombb | tobb;
                        bb[2] ^= frombb | tobb;
                        bb[3] ^= 0;
                        break;
                case ROOK:
                        bb[1] ^= 0;
                        bb[2] ^= 0;
                        bb[3] ^= frombb | tobb;

                        if (from == 7)
                                board->ca &= 0xFD;
                        else if (from == 0)
                                board->ca &= 0xFE;
                        break;
                case QUEEN:
                        bb[1] ^= frombb | tobb;
                        bb[2] ^= 0;
                        bb[3] ^= frombb | tobb;
                        break;
                case KING:
                        bb[2] ^= frombb | tobb;
                        bb[3] ^= frombb | tobb;

                        board->ca &= 0xFD;
                        board->ca &= 0xFE;
                        break;
                }
                break;
        case DPP:
                bb[0] ^= frombb | tobb;
                bb[1] ^= frombb | tobb;

                board->ep = from - 8;
                break;
        case NPC:
                bb[1] &= ~tobb;
                bb[2] &= ~tobb;
                bb[3] &= ~tobb;
        case NP:
                bb[0] ^= frombb | tobb;
                bb[1] ^= frombb | tobb;

                bb[1] |= (KNIGHT & 1) << to;
                bb[2] |= (((KNIGHT) >> 1) & 1) << to;
                bb[3] |= ((KNIGHT) >> 2) << to;
                break;
        case BPC:
                bb[1] &= ~tobb;
                bb[2] &= ~tobb;
                bb[3] &= ~tobb;
        case BP:
                bb[0] ^= frombb | tobb;
                bb[1] ^= frombb | tobb;

                bb[1] |= (BISHOP & 1) << to;
                bb[2] |= (((BISHOP) >> 1) & 1) << to;
                bb[3] |= ((BISHOP) >> 2) << to;
                break;
        case RPC:
                bb[1] &= ~tobb;
                bb[2] &= ~tobb;
                bb[3] &= ~tobb;
        case RP:
                bb[0] ^= frombb | tobb;
                bb[1] ^= frombb | tobb;

                bb[1] |= (ROOK & 1) << to;
                bb[2] |= (((ROOK) >> 1) & 1) << to;
                bb[3] |= ((ROOK) >> 2) << to;
                break;
        case QPC:
                bb[1] &= ~tobb;
                bb[2] &= ~tobb;
                bb[3] &= ~tobb;
        case QP:
                bb[0] ^= frombb | tobb;
                bb[1] ^= frombb | tobb;

                bb[1] |= (QUEEN & 1) << to;
                bb[2] |= (((QUEEN) >> 1) & 1) << to;
                bb[3] |= ((QUEEN) >> 2) << to;
                break;
        case EP:
                bb[0] ^= frombb | tobb;
                bb[1] ^= frombb | tobb;
                bb[1] ^= tobb >> 8;
                break;
        case OO:
                bb[0] ^= frombb | tobb;
                bb[2] ^= frombb | tobb;
                bb[3] ^= frombb | tobb;

                bb[0] ^= 0x00000000000000A0ULL;
                bb[3] ^= 0x00000000000000A0ULL;

                board->ca &= 0xFD;
                break;
        case OOO:
                bb[0] ^= frombb | tobb;
                bb[2] ^= frombb | tobb;
                bb[3] ^= frombb | tobb;

                bb[0] ^= 0x0000000000000009ULL;
                bb[3] ^= 0x0000000000000009ULL;

                board->ca &= 0xFE;
                break;
        default:
                // TODO: Invalid move type
                return;
        }

        board->ep = NOSQ;
        flip(board);
}
