#include "board.h"
#include "bitboard.h"
#include "type.h"

void parse(const char *fen, struct board_t *board)
{
	// TODO: check the validity of the strings

        uint64_t *bb = board->bb;

        int i = 0;
        while (*fen != ' ') {
                switch (*fen) {
                case 'p':
			bb[1] |= ((uint64_t)PAWN & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(PAWN >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)PAWN >> 2) << (i++^0x38);
                        break;
                case 'n':
			bb[1] |= ((uint64_t)KNIGHT & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(KNIGHT >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)KNIGHT >> 2) << (i++^0x38);
                        break;
                case 'b':
			bb[1] |= ((uint64_t)BISHOP & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(BISHOP >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)BISHOP >> 2) << (i++^0x38);
                        break;
                case 'r':
			bb[1] |= ((uint64_t)ROOK & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(ROOK >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)ROOK >> 2) << (i++^0x38);
                        break;
                case 'q':
			bb[1] |= ((uint64_t)QUEEN & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(QUEEN >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)QUEEN >> 2) << (i++^0x38);
                        break;
                case 'k':
			bb[1] |= ((uint64_t)KING & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(KING >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)KING >> 2) << (i++^0x38);
                        break;
                case 'P':
			bb[1] |= ((uint64_t)PAWN & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(PAWN >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)PAWN >> 2) << (i^0x38);
			bb[0] |= 1ULL << (i++^0x38);
                        break;
                case 'N':
			bb[1] |= ((uint64_t)KNIGHT & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(KNIGHT >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)KNIGHT >> 2) << (i^0x38);
			bb[0] |= 1ULL << (i++^0x38);
                        break;
                case 'B':
			bb[1] |= ((uint64_t)BISHOP & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(BISHOP >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)BISHOP >> 2) << (i^0x38);
			bb[0] |= 1ULL << (i++^0x38);
                        break;
                case 'R':
			bb[1] |= ((uint64_t)ROOK & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(ROOK >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)ROOK >> 2) << (i^0x38);
			bb[0] |= 1ULL << (i++^0x38);
                        break;
                case 'Q':
			bb[1] |= ((uint64_t)QUEEN & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(QUEEN >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)QUEEN >> 2) << (i^0x38);
			bb[0] |= 1ULL << (i++^0x38);
                        break;
                case 'K':
			bb[1] |= ((uint64_t)KING & 1) << (i^0x38);
			bb[2] |= ((uint64_t)(KING >> 1) & 1) << (i^0x38);
			bb[3] |= ((uint64_t)KING >> 2) << (i^0x38);
			bb[0] |= 1ULL << (i++^0x38);
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

	if(*fen != '-') {
		for(; *fen != ' '; fen++) {
			if(*fen == 'K') board->ca |= WK;
			else if(*fen == 'Q') board->ca |= WQ;
			else if(*fen == 'k') board->ca |= BK;
			else if(*fen == 'q') board->ca |= BQ;
		}
		fen++;
	} else {
		fen += 2;
	}

	if(*fen != '-') {
		board->ep = 40 + (*fen - 'a');
	}

	if(board->turn == BLACK) {
		flip(&board);
	}
}

void flip(struct board_t *board)
{
	uint64_t *bb = board->bb;

	bb[0] ^= bb[1] | bb[2] | bb[3];
	bb[0] ^= rev(bb[0]);
	bb[1] ^= rev(bb[1]);
	bb[2] ^= rev(bb[2]);
	bb[3] ^= rev(bb[2]);
	board->turn  ^= BLACK;
	board->ca     = (board->ca >> 4) | (board->ca << 4);
}

void make(struct board_t *board, const uint16_t *move) {}

void unmake(struct board_t *board) {}
