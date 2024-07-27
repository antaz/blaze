#include "board.h"
#include "bitboard.h"
#include "move.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void add(struct board_t *board, int type, int idx)
{
    // TODO: Test the speed of this (may potentionally be slower)
    // board->bb[1] |= ((uint64_t)type & 1) << idx;
    // board->bb[2] |= ((uint64_t)(type >> 1) & 1) << idx;
    // board->bb[3] |= ((uint64_t)type >> 2) << idx;

    // This may be faster?
    uint64_t b = 1ULL << idx;
    switch (type) {
    case PAWN:
        board->bb[1] |= b;
        break;
    case KNIGHT:
        board->bb[2] |= b;
        break;
    case BISHOP:
        board->bb[1] |= b;
        board->bb[2] |= b;
        break;
    case ROOK:
        board->bb[3] |= b;
        break;
    case QUEEN:
        board->bb[1] |= b;
        board->bb[3] |= b;
        break;
    case KING:
        board->bb[2] |= b;
        board->bb[3] |= b;
        break;
    }
}

void parse(const char *fen, struct board_t *board)
{
    memset(board->bb, 0, sizeof board->bb);
    board->ca = 0;
    board->ep = NOSQ;
    board->turn = WHITE;
    int turn = WHITE;
    uint64_t *bb = board->bb;

    int i = 0;
    while (*fen != ' ') {
        switch (*fen) {
        case 'p':
            add(board, PAWN, i ^ 0x38);
            i++;
            break;
        case 'n':
            add(board, KNIGHT, i ^ 0x38);
            i++;
            break;
        case 'b':
            add(board, BISHOP, i ^ 0x38);
            i++;
            break;
        case 'r':
            add(board, ROOK, i ^ 0x38);
            i++;
            break;
        case 'q':
            add(board, QUEEN, i ^ 0x38);
            i++;
            break;
        case 'k':
            add(board, KING, i ^ 0x38);
            i++;
            break;
        case 'P':
            add(board, PAWN, i ^ 0x38);
            bb[0] |= 1ULL << (i++ ^ 0x38);
            break;
        case 'N':
            add(board, KNIGHT, i ^ 0x38);
            bb[0] |= 1ULL << (i++ ^ 0x38);
            break;
        case 'B':
            add(board, BISHOP, i ^ 0x38);
            bb[0] |= 1ULL << (i++ ^ 0x38);
            break;
        case 'R':
            add(board, ROOK, i ^ 0x38);
            bb[0] |= 1ULL << (i++ ^ 0x38);
            break;
        case 'Q':
            add(board, QUEEN, i ^ 0x38);
            bb[0] |= 1ULL << (i++ ^ 0x38);
            break;
        case 'K':
            add(board, KING, i ^ 0x38);
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

    turn = *fen == 'w' ? WHITE : BLACK;

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
        board->ep = *fen - 'a';
    }

    if (turn == BLACK) {
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
    bb[0] = vflip(bb[0]);
    bb[1] = vflip(bb[1]);
    bb[2] = vflip(bb[2]);
    bb[3] = vflip(bb[3]);
    board->turn ^= BLACK;
    board->ca = (board->ca >> 4) | (board->ca << 4);
}

void clear(struct board_t *board, const uint8_t idx)
{
    uint64_t b = 1ULL << idx;
    board->bb[1] &= ~b;
    board->bb[2] &= ~b;
    board->bb[3] &= ~b;
}

void make(struct board_t *board, const uint16_t move)
{
    uint64_t *bb, frombb, tobb;
    uint8_t from, to, piece, cap;

    bb = board->bb;
    from = MOVE_FROM(move);
    to = MOVE_TO(move);
    frombb = 1ULL << from;
    tobb = 1ULL << to;

    board->hist[board->ply].ep = board->ep;
    board->hist[board->ply].ca = board->ca;
    board->hist[board->ply].cap = EMPTY;
    board->ep = NOSQ;

    if (move & 0x4000) {
        // move is a capture

        // track the captured piece
        cap = ((bb[3] >> (to)) & 1) << 2 | ((bb[2] >> (to)) & 1) << 1 |
              ((bb[1] >> (to)) & 1);
        assert(cap != KING);
        board->hist[board->ply].cap = cap;

        // clear the destination square if it's a capture
        // effectively removing the captured piece
        // note we don't clear the bb[0] board (i.e side to move)
        // because the captured piece is our opponent's piece'
        clear(board, to);

        // reset castling if one of the rooks were captured
        if (to == 63)
            board->ca &= 0xDF;
        else if (to == 56)
            board->ca &= 0xEF;
    } else {
        // move is not a capture
    }

    bb[0] ^= frombb | tobb;

    switch (MOVE_TYPE(move)) {
    case CAPTURE:
    case QUIET:
        piece = ((bb[3] >> (from)) & 1) << 2 | ((bb[2] >> (from)) & 1) << 1 |
                ((bb[1] >> (from)) & 1);

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
        bb[1] ^= frombb | tobb;

        // check if en passant is possible
        uint64_t their_pawns =
            (bb[1] & ~bb[2] & ~bb[3]) & (bb[0] ^ (bb[1] | bb[2] | bb[3]));
        uint64_t p = ((1ULL << ((to & 7) + 16)) << 7 & ~FH) |
                     ((1ULL << ((to & 7) + 16)) << 9 & ~FA);
        if (to == from + 16 && p & their_pawns) {
            board->ep = to & 0x07;
        }
        break;
    case NPC:
    case NP:
        bb[1] ^= frombb;
        add(board, KNIGHT, to);
        break;
    case BPC:
    case BP:
        bb[1] ^= frombb;
        add(board, BISHOP, to);
        break;
    case RPC:
    case RP:
        bb[1] ^= frombb;
        add(board, ROOK, to);
        break;
    case QPC:
    case QP:
        bb[1] ^= frombb;
        add(board, QUEEN, to);
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

    board->ply++;
    flip(board);
}

void unmake(struct board_t *board, const uint16_t move)
{
    uint64_t *bb, frombb, tobb;
    uint8_t to, piece;

    board->ply--;
    flip(board);
    bb = board->bb;
    // from = MOVE_FROM(move);
    to = MOVE_TO(move);
    frombb = 1ULL << ((move >> 6) & 0x3f);
    tobb = 1ULL << (move & 0x3f);

    board->ep = board->hist[board->ply].ep;
    board->ca = board->hist[board->ply].ca;

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

        clear(board, to);
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
        // clear the destination square
        clear(board, to);

        // find the captured piece and put it back in place
        add(board, board->hist[board->ply].cap, to);
    }
}
