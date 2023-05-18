#include "board.h"
#include "bitboard.h"
#include "type.h"

void parse(const char *fen, struct board_t *board)
{
        uint64_t *bb = board->bb;
        uint8_t *mail = board->mail;

        int i = 7 * 8; // TODO: don't hard code this
        while (*fen) {
                switch (*fen) {
                case 'p':
                        set(bb[BLACK], i);
                        mail[i] = piece(BLACK, PAWN);
                        set(bb[PAWN], i++);
                        break;
                case 'n':
                        set(bb[BLACK], i);
                        mail[i] = piece(BLACK, KNIGHT);
                        set(bb[KNIGHT], i++);
                        break;
                case 'b':
                        set(bb[BLACK], i);
                        mail[i] = piece(BLACK, BISHOP);
                        set(bb[BISHOP], i++);
                        break;
                case 'r':
                        set(bb[BLACK], i);
                        mail[i] = piece(BLACK, ROOK);
                        set(bb[ROOK], i++);
                        break;
                case 'q':
                        set(bb[BLACK], i);
                        mail[i] = piece(BLACK, QUEEN);
                        set(bb[QUEEN], i++);
                        break;
                case 'k':
                        set(bb[BLACK], i);
                        mail[i] = piece(BLACK, KING);
                        set(bb[KING], i++);
                        break;
                case 'P':
                        set(bb[WHITE], i);
                        mail[i] = piece(WHITE, PAWN);
                        set(bb[PAWN], i++);
                        break;
                case 'N':
                        set(bb[WHITE], i);
                        mail[i] = piece(WHITE, KNIGHT);
                        set(bb[KNIGHT], i++);
                        break;
                case 'B':
                        set(bb[WHITE], i);
                        mail[i] = piece(WHITE, BISHOP);
                        set(bb[BISHOP], i++);
                        break;
                case 'R':
                        set(bb[WHITE], i);
                        mail[i] = piece(WHITE, ROOK);
                        set(bb[ROOK], i++);
                        break;
                case 'Q':
                        set(bb[WHITE], i);
                        mail[i] = piece(WHITE, QUEEN);
                        set(bb[QUEEN], i++);
                        break;
                case 'K':
                        set(bb[WHITE], i);
                        mail[i] = piece(WHITE, KING);
                        set(bb[KING], i++);
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
                        i -= 2 * 8;
                case ' ':
                        ++fen;
                        continue;

                default:
                        break;
                }
                ++fen;
        }
}

void make(struct board_t *board, const uint16_t *move) {}

void unmake(struct board_t *board) {}
