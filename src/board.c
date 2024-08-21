#include "board.h"
#include "bitboard.h"
#include "hash.h"
#include "move.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint64_t piece_hash[2][PC][SQ];
uint64_t ep_hash[8];
uint64_t ca_hash[52];
uint64_t stm_hash;

void add(uint64_t *bb, const int tp, const int idx)
{
    // TODO: Test the speed of this (may potentionally be slower)
    // board->bb[1] |= ((uint64_t)type & 1) << idx;
    // board->bb[2] |= ((uint64_t)(type >> 1) & 1) << idx;
    // board->bb[3] |= ((uint64_t)type >> 2) << idx;

    // This may be faster?
    uint64_t b = 1ULL << idx;
    switch (tp) {
    case PAWN:
        bb[1] |= b;
        break;
    case KNIGHT:
        bb[2] |= b;
        break;
    case BISHOP:
        bb[1] |= b;
        bb[2] |= b;
        break;
    case ROOK:
        bb[3] |= b;
        break;
    case QUEEN:
        bb[1] |= b;
        bb[3] |= b;
        break;
    case KING:
        bb[2] |= b;
        bb[3] |= b;
        break;
    }
}

void cls(uint64_t *bb, const int idx)
{
    uint64_t b = 1ULL << idx;
    bb[1] &= ~b;
    bb[2] &= ~b;
    bb[3] &= ~b;
}

void movep(struct board_t *board, int fidx, int tidx)
{
    uint64_t *bb = board->bb;
    uint64_t fb = 1ULL << fidx;
    uint64_t tb = 1ULL << tidx;

    int pf = ((bb[3] >> (fidx)) & 1) << 2 | ((bb[2] >> (fidx)) & 1) << 1 |
             ((bb[1] >> (fidx)) & 1);

    int pt = ((bb[3] >> (tidx)) & 1) << 2 | ((bb[2] >> (tidx)) & 1) << 1 |
             ((bb[1] >> (tidx)) & 1);

    switch (pf) {
    case PAWN:
        bb[1] ^= fb | tb;
        break;
    case KNIGHT:
        bb[1] ^= 0;
        bb[2] ^= fb | tb;
        bb[3] ^= 0;
        break;
    case BISHOP:
        bb[1] ^= fb | tb;
        bb[2] ^= fb | tb;
        bb[3] ^= 0;
        break;
    case ROOK:
        bb[1] ^= 0;
        bb[2] ^= 0;
        bb[3] ^= fb | tb;
        break;
    case QUEEN:
        bb[1] ^= fb | tb;
        bb[2] ^= 0;
        bb[3] ^= fb | tb;
        break;
    case KING:
        bb[2] ^= fb | tb;
        bb[3] ^= fb | tb;
        break;
    }

    // update hash
    // board->hash ^= piece_hash[stm][pf][fidx] ^ piece_hash[stm][pf][tidx] ^
    //                piece_hash[stm ^ BLACK][pt][tidx];
}

void parse(struct board_t *board, const char *fen)
{
    memset(board->bb, 0, sizeof board->bb);
    board->ca = 0;
    board->ep = NOSQ;
    board->stm = WHITE;
    board->ply = 0;
    board->hply = 0;
    board->fifty = 0;
    board->hash = 0ULL;
    int stm = WHITE;
    uint64_t *bb = board->bb;

    int i = 0;
    while (*fen != ' ') {
        switch (*fen) {
        case 'p':
            add(board->bb, PAWN, i ^ 0x38);
            board->hash ^= piece_hash[BLACK][PAWN][i ^ 0x38];
            i++;
            break;
        case 'n':
            add(board->bb, KNIGHT, i ^ 0x38);
            board->hash ^= piece_hash[BLACK][KNIGHT][i ^ 0x38];
            i++;
            break;
        case 'b':
            add(board->bb, BISHOP, i ^ 0x38);
            board->hash ^= piece_hash[BLACK][BISHOP][i ^ 0x38];
            i++;
            break;
        case 'r':
            add(board->bb, ROOK, i ^ 0x38);
            board->hash ^= piece_hash[BLACK][ROOK][i ^ 0x38];
            i++;
            break;
        case 'q':
            add(board->bb, QUEEN, i ^ 0x38);
            board->hash ^= piece_hash[BLACK][QUEEN][i ^ 0x38];
            i++;
            break;
        case 'k':
            add(board->bb, KING, i ^ 0x38);
            board->hash ^= piece_hash[BLACK][KING][i ^ 0x38];
            i++;
            break;
        case 'P':
            add(board->bb, PAWN, i ^ 0x38);
            bb[0] |= 1ULL << (i ^ 0x38);
            board->hash ^= piece_hash[WHITE][PAWN][i ^ 0x38];
            i++;
            break;
        case 'N':
            add(board->bb, KNIGHT, i ^ 0x38);
            bb[0] |= 1ULL << (i ^ 0x38);
            board->hash ^= piece_hash[WHITE][KNIGHT][i ^ 0x38];
            i++;
            break;
        case 'B':
            add(board->bb, BISHOP, i ^ 0x38);
            bb[0] |= 1ULL << (i ^ 0x38);
            board->hash ^= piece_hash[WHITE][BISHOP][i ^ 0x38];
            i++;
            break;
        case 'R':
            add(board->bb, ROOK, i ^ 0x38);
            bb[0] |= 1ULL << (i ^ 0x38);
            board->hash ^= piece_hash[WHITE][ROOK][i ^ 0x38];
            i++;
            break;
        case 'Q':
            add(board->bb, QUEEN, i ^ 0x38);
            bb[0] |= 1ULL << (i ^ 0x38);
            board->hash ^= piece_hash[WHITE][QUEEN][i ^ 0x38];
            i++;
            break;
        case 'K':
            add(board->bb, KING, i ^ 0x38);
            bb[0] |= 1ULL << (i ^ 0x38);
            board->hash ^= piece_hash[WHITE][KING][i ^ 0x38];
            i++;
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

    stm = *fen == 'w' ? WHITE : BLACK;

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
        // board->hash ^= ca_hash[board->ca];
    } else {
        fen += 2;
    }

    if (*fen != '-') {
        board->ep = *fen - 'a';
        // board->hash ^= ep_hash[*fen - 'a'];
        fen++;
    } else {
        fen += 2;
    }

    if (stm == BLACK) {
        board->hash ^= stm_hash;
        flip(board);
    }
}

void flip(struct board_t *board)
{
    uint64_t *bb = board->bb;
    uint8_t *stm = &board->stm;
    uint8_t *ca = &board->ca;

    bb[0] ^= bb[1] | bb[2] | bb[3];
    bb[0] = vflip(bb[0]);
    bb[1] = vflip(bb[1]);
    bb[2] = vflip(bb[2]);
    bb[3] = vflip(bb[3]);
    *stm ^= BLACK;
    *ca = (board->ca >> 4) | (board->ca << 4);
}

void make(struct board_t *board, const uint16_t move)
{
    uint64_t *bb, frombb, tobb;
    int from, to;

    bb = board->bb;
    from = MOVE_FROM(move);
    to = MOVE_TO(move);
    int ply = board->ply;
    int hply = board->hply;
    int stm = board->stm;
    frombb = 1ULL << from;
    tobb = 1ULL << to;

    board->hist[hply].ep = board->ep;
    board->hist[hply].ca = board->ca;
    board->hist[hply].fifty = board->fifty;
    board->hist[hply].cap = EMPTY;
    board->hist[hply].hash = board->hash;
    board->ep = NOSQ;

    int piece = ((bb[3] >> (from)) & 1) << 2 | ((bb[2] >> (from)) & 1) << 1 |
                ((bb[1] >> (from)) & 1);

    if (move & 0x4000) {
        // move is a capture
        // track the captured piece
        int cap = ((bb[3] >> (to)) & 1) << 2 | ((bb[2] >> (to)) & 1) << 1 |
                  ((bb[1] >> (to)) & 1);
        assert(cap != KING);
        board->hist[ply].cap = cap;

        // clear the destination square if it's a capture
        // effectively removing the captured piece
        // note we don't clear the bb[0] board (i.e side to move)
        // because the captured piece is our opponent's piece'
        cls(bb, to);

        // reset castling if one of the rooks were captured
        if (to == 63)
            board->ca &= 0xDF;
        else if (to == 56)
            board->ca &= 0xEF;

        // reset 50 move counter
        board->fifty = 0;

        // hash the from and to square
        if (stm == WHITE) {
            board->hash ^= piece_hash[stm][piece][from] ^
                           piece_hash[stm][piece][to] ^
                           piece_hash[stm ^ BLACK][cap][to];
        } else {
            board->hash ^= piece_hash[stm][piece][from ^ 0x38] ^
                           piece_hash[stm][piece][to ^ 0x38] ^
                           piece_hash[stm ^ BLACK][cap][to ^ 0x38];
        }
    } else {
        // move is not a capture

        // increment 50 move counter
        board->fifty++;

        // hash the from and to square
        if (stm == WHITE) {
            board->hash ^=
                piece_hash[stm][piece][from] ^ piece_hash[stm][piece][to];
        } else {
            board->hash ^= piece_hash[stm][piece][from ^ 0x38] ^
                           piece_hash[stm][piece][to ^ 0x38];
        }
    }

    bb[0] ^= frombb | tobb;

    switch (MOVE_TYPE(move)) {
    case CAPTURE:
    case QUIET:
        switch (piece) {
        case PAWN:
            bb[1] ^= frombb | tobb;

            board->fifty = 0;
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
        bb[1] ^= frombb | tobb;

        // check if en passant is possible
        uint64_t their_pawns =
            (bb[1] & ~bb[2] & ~bb[3]) & (bb[0] ^ (bb[1] | bb[2] | bb[3]));
        uint64_t p = ((1ULL << ((to & 7) + 16)) << 7 & ~FH) |
                     ((1ULL << ((to & 7) + 16)) << 9 & ~FA);
        if (to == from + 16 && p & their_pawns) {
            board->ep = to & 0x07;
        }

        board->fifty = 0;
        break;
    case NPC:
    case NP:
        bb[1] ^= frombb;
        add(board->bb, KNIGHT, to);
        break;
    case BPC:
    case BP:
        bb[1] ^= frombb;
        add(board->bb, BISHOP, to);
        break;
    case RPC:
    case RP:
        bb[1] ^= frombb;
        add(board->bb, ROOK, to);
        break;
    case QPC:
    case QP:
        bb[1] ^= frombb;
        add(board->bb, QUEEN, to);
        break;
    case EP:
        bb[1] ^= frombb | tobb;
        bb[1] ^= tobb >> 8;
        break;
    case OO:
        bb[2] ^= frombb | tobb;
        bb[3] ^= frombb | tobb;

        bb[0] ^= 0x00000000000000A0ULL;
        bb[3] ^= 0x00000000000000A0ULL;

        board->ca &= 0xFD;
        board->ca &= 0xFE;
        break;
    case OOO:
        bb[2] ^= frombb | tobb;
        bb[3] ^= frombb | tobb;

        bb[0] ^= 0x0000000000000009ULL;
        bb[3] ^= 0x0000000000000009ULL;

        board->ca &= 0xFD;
        board->ca &= 0xFE;
        break;
    default:
        // TODO: Invalid move type
        return;
    }

    board->hash ^= stm_hash;
    // board->hash ^= ep_hash[board->ep];
    // board->hash ^= ca_hash[board->ca];
    board->ply++;
    board->hply++;
    flip(board);
}

void take(struct board_t *board, const uint16_t move)
{
    uint64_t *bb, frombb, tobb;
    uint8_t from, to, piece;

    board->ply--;
    board->hply--;
    flip(board);
    bb = board->bb;
    int stm = board->stm;
    from = MOVE_FROM(move);
    to = MOVE_TO(move);
    frombb = 1ULL << from;
    tobb = 1ULL << to;

    board->ep = board->hist[board->hply].ep;
    board->fifty = board->hist[board->hply].fifty;
    board->ca = board->hist[board->hply].ca;
    int cap = board->hist[board->hply].cap;
    board->hash = board->hist[board->hply].hash;

    bb[0] ^= frombb | tobb;

    switch (MOVE_TYPE(move)) {
    case CAPTURE:
    case QUIET:
        piece = ((bb[3] >> (to)) & 1) << 2 | ((bb[2] >> (to)) & 1) << 1 |
                ((bb[1] >> (to)) & 1);

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
            break;
        case QUEEN:
            bb[1] ^= frombb | tobb;
            bb[2] ^= 0;
            bb[3] ^= frombb | tobb;
            break;
        case KING:
            bb[2] ^= frombb | tobb;
            bb[3] ^= frombb | tobb;
            break;
        }
        break;
    case DPP:
    case NPC:
    case BPC:
    case RPC:
    case QPC:
    case NP:
    case BP:
    case RP:
    case QP:
        bb[1] ^= frombb | tobb;

        cls(bb, to);
        break;
    case EP:
        bb[1] ^= frombb | tobb;
        bb[1] ^= tobb >> 8;
        break;
    case OO:
        bb[2] ^= frombb | tobb;
        bb[3] ^= frombb | tobb;

        bb[0] ^= 0x00000000000000A0ULL;
        bb[3] ^= 0x00000000000000A0ULL;
        break;
    case OOO:
        bb[2] ^= frombb | tobb;
        bb[3] ^= frombb | tobb;

        bb[0] ^= 0x0000000000000009ULL;
        bb[3] ^= 0x0000000000000009ULL;
        break;
    default:
        // TODO: Invalid move type
        return;
    }

    if (move & 0x4000) {
        // move is a capture
        int piece = ((bb[3] >> (from)) & 1) << 2 |
                    ((bb[2] >> (from)) & 1) << 1 | ((bb[1] >> (from)) & 1);

        // clear the destination square
        cls(bb, to);

        // find the captured piece and put it back in place
        add(board->bb, board->hist[board->ply].cap, to);

    } else {
        // move is not a capture
        int piece = ((bb[3] >> (from)) & 1) << 2 |
                    ((bb[2] >> (from)) & 1) << 1 | ((bb[1] >> (from)) & 1);
    }
}
