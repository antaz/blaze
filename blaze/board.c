#include "bitboard.h"
#include "board.h"
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
