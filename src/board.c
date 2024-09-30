#include "board.h"
#include "bitboard.h"
#include "hash.h"
#include "move.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>

uint64_t piece_hash[2][PC][SQ];
uint64_t ep_hash[9];
uint64_t ca_hash[52];
uint64_t stm_hash;

static void reset(struct board_t *board)
{
	memset(board->bb, 0, sizeof board->bb);
	memset(board->hist, 0, sizeof board->hist);
	board->stm = WHITE;
	board->ply = 0;
	board->hply = 0;
	board->fifty = 0;
	board->ca = 0;
	board->ep = NOSQ;
	board->hash = 0ULL;
}

void parse(struct board_t *board, const char *fen)
{
	// reset the board
	reset(board);

	// initialize variables
	int i = 0;
	int stm = WHITE;
	uint64_t *bb = board->bb;

	// parse FEN pieces
	while (*fen != ' ') {
		int sq = i ^ 0x38;

		switch (*fen) {
		case 'p':
			bb[1] |= 1ULL << sq;
			i++;
			break;
		case 'n':
			bb[2] |= 1ULL << sq;
			i++;
			break;
		case 'b':
			bb[1] |= 1ULL << sq;
			bb[2] |= 1ULL << sq;
			i++;
			break;
		case 'r':
			bb[3] |= 1ULL << sq;
			i++;
			break;
		case 'q':
			bb[1] |= 1ULL << sq;
			bb[3] |= 1ULL << sq;
			i++;
			break;
		case 'k':
			bb[2] |= 1ULL << sq;
			bb[3] |= 1ULL << sq;
			i++;
			break;
		case 'P':
			bb[1] |= 1ULL << sq;
			bb[0] |= 1ULL << sq;
			i++;
			break;
		case 'N':
			bb[2] |= 1ULL << sq;
			bb[0] |= 1ULL << sq;
			i++;
			break;
		case 'B':
			bb[1] |= 1ULL << sq;
			bb[2] |= 1ULL << sq;
			bb[0] |= 1ULL << sq;
			i++;
			break;
		case 'R':
			bb[3] |= 1ULL << sq;
			bb[0] |= 1ULL << sq;
			i++;
			break;
		case 'Q':
			bb[1] |= 1ULL << sq;
			bb[3] |= 1ULL << sq;
			bb[0] |= 1ULL << sq;
			i++;
			break;
		case 'K':
			bb[2] |= 1ULL << sq;
			bb[3] |= 1ULL << sq;
			bb[0] |= 1ULL << sq;
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

	// parse side to move
	stm = *fen == 'w' ? WHITE : BLACK;
	fen += 2;

	// parse castling permissions
	if (*fen != '-') {
		for (; *fen != ' '; fen++) {
			switch (*fen) {
			case 'K':
				board->ca |= K;
				break;
			case 'Q':
				board->ca |= Q;
				break;
			case 'k':
				board->ca |= OK;
				break;
			case 'q':
				board->ca |= OQ;
				break;
			}
		}
		fen++;
	} else {
		fen += 2;
	}

	// parse en-passant square
	if (*fen != '-') {
		board->ep = *fen - 'a';
		fen++;
	} else {
		fen += 2;
	}

	// flip the board to the side to move
	if (stm == BLACK) {
		flip(board);
	}

	// calculate hash
	board->hash = zobrist(board);
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
	*ca = (*ca >> 2) ^ ((*ca & 3) << 2);
}

void make(struct board_t *board, const uint16_t move)
{
	// initialize variables
	int from = MOVE_FROM(move);
	int to = MOVE_TO(move);
	uint64_t *bb = board->bb;
	uint64_t fb = 1ULL << from;
	uint64_t tb = 1ULL << to;
	int hply = board->hply;

	// store history
	board->hist[hply].ep = board->ep;
	board->hist[hply].ca = board->ca;
	board->hist[hply].fifty = board->fifty;
	board->hist[hply].hash = board->hash;

	// reset en-passant column
	board->ep = NOSQ;

	// moving piece
	int p = ((bb[3] >> (from)) & 1) << 2 | ((bb[2] >> (from)) & 1) << 1 |
		((bb[1] >> (from)) & 1);

	// check if the move is a capture
	if (move & 0x4000) {
		// track the captured piece in history
		int cap = ((bb[3] >> (to)) & 1) << 2 |
			  ((bb[2] >> (to)) & 1) << 1 | ((bb[1] >> (to)) & 1);
		board->hist[hply].cap = cap;

		// clear the destination square
		bb[1] &= ~tb;
		bb[2] &= ~tb;
		bb[3] &= ~tb;

		// reset castling if one of the rooks was captured
		if (to == 63)
			board->ca &= 11;
		else if (to == 56)
			board->ca &= 7;

		// reset 50 move counter
		board->fifty = 0;
	} else {
		// increment 50 move counter
		board->fifty++;
	}

	switch (MOVE_TYPE(move)) {
	case CAPTURE:
	case QUIET:
		switch (p) {
		case PAWN:
			bb[0] ^= fb | tb;
			bb[1] ^= fb | tb;

			// reset fifity-move counter
			board->fifty = 0;
			break;
		case KNIGHT:
			bb[0] ^= fb | tb;
			bb[1] ^= 0;
			bb[2] ^= fb | tb;
			bb[3] ^= 0;
			break;
		case BISHOP:
			bb[0] ^= fb | tb;
			bb[1] ^= fb | tb;
			bb[2] ^= fb | tb;
			bb[3] ^= 0;
			break;
		case ROOK:
			bb[0] ^= fb | tb;
			bb[1] ^= 0;
			bb[2] ^= 0;
			bb[3] ^= fb | tb;

			// reset castling permissions if one of the rooks moved
			if (from == 7)
				board->ca &= 14;
			else if (from == 0)
				board->ca &= 13;
			break;
		case QUEEN:
			bb[0] ^= fb | tb;
			bb[1] ^= fb | tb;
			bb[2] ^= 0;
			bb[3] ^= fb | tb;
			break;
		case KING:
			bb[0] ^= fb | tb;
			bb[2] ^= fb | tb;
			bb[3] ^= fb | tb;

			// reset castling permissions
			board->ca &= 12;
			break;
		}
		break;
	case DPP:
		bb[0] ^= fb | tb;
		bb[1] ^= fb | tb;

		// opponent's pawns that can be captured en-passant
		uint64_t p = bb[1] & ~bb[2] & ~bb[3] & ~bb[0];
		p &= ((1ULL << ((to & 7) + 16)) << 7 & ~FH) |
		     ((1ULL << ((to & 7) + 16)) << 9 & ~FA);

		// check if en passant is possible
		if (to == from + 16 && p) {
			// track the en-passant column
			board->ep = to & 0x07;
		}

		// reset fifty move counter
		board->fifty = 0;
		break;
	case NPC:
	case NP:
		bb[0] ^= fb | tb;
		bb[1] ^= fb;
		bb[2] |= tb;
		break;
	case BPC:
	case BP:
		bb[0] ^= fb | tb;
		bb[1] ^= fb;
		bb[1] |= tb;
		bb[2] |= tb;
		break;
	case RPC:
	case RP:
		bb[0] ^= fb | tb;
		bb[1] ^= fb;
		bb[3] |= tb;
		break;
	case QPC:
	case QP:
		bb[0] ^= fb | tb;
		bb[1] ^= fb;
		bb[1] |= tb;
		bb[3] |= tb;
		break;
	case EP:
		bb[0] ^= fb | tb;
		bb[1] ^= fb | tb;
		bb[1] ^= tb >> 8;
		break;
	case OO:
		// move the king
		bb[0] ^= fb | tb;
		bb[2] ^= fb | tb;
		bb[3] ^= fb | tb;

		// move the rook
		bb[0] ^= 0x00000000000000A0ULL;
		bb[3] ^= 0x00000000000000A0ULL;

		// reset castling permissions
		board->ca &= 12;
		break;
	case OOO:
		// move the king
		bb[0] ^= fb | tb;
		bb[2] ^= fb | tb;
		bb[3] ^= fb | tb;

		// move the rook
		bb[0] ^= 0x0000000000000009ULL;
		bb[3] ^= 0x0000000000000009ULL;

		// reset castling permissions
		board->ca &= 12;
		break;
	default:
		return;
	}

	// increment counter
	board->ply++;
	board->hply++;

	// calculate hash
	board->hash = zobrist(board);

	// flip the board
	flip(board);
}

void take(struct board_t *board, const uint16_t move)
{
	// initialize variables
	int from = MOVE_FROM(move);
	int to = MOVE_TO(move);
	uint64_t *bb = board->bb;
	uint64_t fb = 1ULL << from;
	uint64_t tb = 1ULL << to;

	// decrement counters
	board->ply--;
	board->hply--;

	// flip the board
	flip(board);

	// retrieve history
	board->ep = board->hist[board->hply].ep;
	board->ca = board->hist[board->hply].ca;
	board->fifty = board->hist[board->hply].fifty;
	board->hash = board->hist[board->hply].hash;

	// piece to move
	int p = ((bb[3] >> (to)) & 1) << 2 | ((bb[2] >> (to)) & 1) << 1 |
		((bb[1] >> (to)) & 1);

	switch (MOVE_TYPE(move)) {
	case CAPTURE:
	case QUIET:
		switch (p) {
		case PAWN:
			bb[0] ^= fb | tb;
			bb[1] ^= fb | tb;
			break;
		case KNIGHT:
			bb[0] ^= fb | tb;
			bb[1] ^= 0;
			bb[2] ^= fb | tb;
			bb[3] ^= 0;
			break;
		case BISHOP:
			bb[0] ^= fb | tb;
			bb[1] ^= fb | tb;
			bb[2] ^= fb | tb;
			bb[3] ^= 0;
			break;
		case ROOK:
			bb[0] ^= fb | tb;
			bb[1] ^= 0;
			bb[2] ^= 0;
			bb[3] ^= fb | tb;
			break;
		case QUEEN:
			bb[0] ^= fb | tb;
			bb[1] ^= fb | tb;
			bb[2] ^= 0;
			bb[3] ^= fb | tb;
			break;
		case KING:
			bb[0] ^= fb | tb;
			bb[2] ^= fb | tb;
			bb[3] ^= fb | tb;
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
		bb[0] ^= fb | tb;
		bb[1] ^= fb | tb;

		bb[1] &= ~tb;
		bb[2] &= ~tb;
		bb[3] &= ~tb;
		break;
	case EP:
		bb[0] ^= fb | tb;
		bb[1] ^= fb | tb;
		bb[1] ^= tb >> 8;
		break;
	case OO:
		// move the king
		bb[0] ^= fb | tb;
		bb[2] ^= fb | tb;
		bb[3] ^= fb | tb;

		// move the rook
		bb[0] ^= 0x00000000000000A0ULL;
		bb[3] ^= 0x00000000000000A0ULL;
		break;
	case OOO:
		// move the king
		bb[0] ^= fb | tb;
		bb[2] ^= fb | tb;
		bb[3] ^= fb | tb;

		// move the rook
		bb[0] ^= 0x0000000000000009ULL;
		bb[3] ^= 0x0000000000000009ULL;
		break;
	default:
		return;
	}

	// check if move is a capture
	if (move & 0x4000) {
		// clear the destination square
		bb[1] &= ~tb;
		bb[2] &= ~tb;
		bb[3] &= ~tb;

		// add the captured piece back in place
		int cap = board->hist[board->hply].cap;
		switch (cap) {
		case PAWN:
			bb[1] |= 1ULL << to;
			break;
		case KNIGHT:
			bb[2] |= 1ULL << to;
			break;
		case BISHOP:
			bb[1] |= 1ULL << to;
			bb[2] |= 1ULL << to;
			break;
		case ROOK:
			bb[3] |= 1ULL << to;
			break;
		case QUEEN:
			bb[1] |= 1ULL << to;
			bb[3] |= 1ULL << to;
			break;
		case KING:
			bb[2] |= 1ULL << to;
			bb[3] |= 1ULL << to;
			break;
		}
	}
}
