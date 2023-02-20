#include "definitions.h"
#include "functions.h"
#include <assert.h>
#include <stdio.h>

#include "board.h"

const int knightDir[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int rookDir[4] = {-1, -10, 1, 10};
const int bishopDir[4] = {-9, -11, 11, 9};
const int kingDir[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

int isAttacked(struct board_t *board, int square, int turn)
{
        int i, piece, t_square, dir;
        assert(onBoard(square));
        assert(turn == WHITE || turn == BLACK);

        // Checking pawn attacks
        if (turn == WHITE) {
                if (board->pieces[square - 11] == wP ||
                    board->pieces[square - 9] == wP)
                        return 1;
        } else {
                if (board->pieces[square + 11] == bP ||
                    board->pieces[square + 9] == bP)
                        return 1;
        }
        // Checking knight attacks

        for (i = 0; i < 8; i++) {
                piece = board->pieces[square + knightDir[i]];
                if (piece != OFFBOARD && isKnight(piece) &&
                    pieceColor(piece) == turn)
                        return 1;
        }

        // Checking Rooks and Queens attacks
        for (i = 0; i < 4; i++) {
                dir = rookDir[i];                // dir = -1
                t_square = square + dir;         // t_square = 55 - 1 = 54
                piece = board->pieces[t_square]; // piece = board->piece[54]
                while (piece != OFFBOARD) {
                        if (piece != EMPTY) {
                                if ((isRook(piece) || isQueen(piece)) &&
                                    pieceColor(piece) == turn) {
                                        return 1;
                                }
                                break;
                        }
                        t_square += dir;
                        piece = board->pieces[t_square];
                }
        }

        // Checking Rooks and Queens attacks
        for (i = 0; i < 4; i++) {
                dir = bishopDir[i];
                t_square = square + dir;
                piece = board->pieces[t_square];
                while (piece != OFFBOARD) {
                        if (piece != EMPTY) {
                                if ((isBishop(piece) || isQueen(piece)) &&
                                    pieceColor(piece) == turn) {
                                        return 1;
                                }
                                break;
                        }
                        t_square += dir;
                        piece = board->pieces[t_square];
                }
        }

        // Checking king attacks
        for (i = 0; i < 8; i++) {
                piece = board->pieces[square + kingDir[i]];
                if (piece != OFFBOARD && isKing(piece) &&
                    pieceColor(piece) == turn) {
                        return 1;
                }
        }

        return 0;
}
