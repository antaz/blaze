#include "definitions.h"
#include "functions.h"
#include <stdio.h>

const int piece_square_table[2][6][64] = {
    {{
         // Pawns - early/mid
         //  A   B   C   D   E   F   G   H
         0,   0,   0,   0,   0,   0,  0,   0,  // 1
         -1,  -7,  -11, -35, -13, 5,  3,   -5, // 2
         1,   1,   -6,  -19, -6,  -7, -4,  10, // 3
         1,   14,  8,   4,   5,   4,  10,  7,  // 4
         9,   30,  23,  31,  31,  23, 17,  11, // 5
         21,  54,  72,  56,  77,  95, 71,  11, // 6
         118, 121, 173, 168, 107, 82, -16, 22, // 7
         0,   0,   0,   0,   0,   0,  0,   0   // 8
     },
     {
         // Knights - early/mid
         //  A   B   C   D   E   F   G   H
         -99, -30, -66, -64, -29, -19, -61, -81, // 1
         -56, -31, -28, -1,  -7,  -20, -42, -11, // 2
         -38, -16, 0,   14,  8,   3,   3,   -42, // 3
         -14, 0,   2,   3,   19,  12,  33,  -7,  // 4
         -14, -4,  25,  33,  10,  33,  14,  43,  // 5
         -22, 18,  60,  64,  124, 143, 55,  6,   // 6
         -34, 24,  54,  74,  60,  122, 2,   29,  // 7
         -60, 0,   0,   0,   0,   0,   0,   0    // 8
     },
     {
         // Bishops - early/mid
         //  A   B   C   D   E   F   G   H
         -7,  12,  -8, -37, -31, -8,  -45, -67, // 1
         15,  5,   13, -10, 1,   2,   0,   15,  // 2
         5,   12,  14, 13,  10,  -1,  3,   4,   // 3
         1,   5,   23, 32,  21,  8,   17,  4,   // 4
         -1,  16,  29, 27,  37,  27,  17,  4,   // 5
         7,   27,  20, 56,  91,  108, 53,  44,  // 6
         -24, -23, 30, 58,  65,  61,  69,  11,  // 7
         0,   0,   0,  0,   0,   0,   0,   0    // 8
     },
     {
         // Rooks - early/mid
         //  A   B   C   D   E   F   G   H
         -2,  -1, 3,  1,  2,   1,   4,  -8,  // 1
         -26, -6, 2,  -2, 2,   -10, -1, -29, // 2
         -16, 0,  3,  -3, 8,   -1,  12, 3,   // 3
         -9,  -5, 8,  14, 18,  -17, 13, -13, // 4
         19,  33, 46, 57, 53,  39,  53, 16,  // 5
         24,  83, 54, 75, 134, 144, 85, 75,  // 6
         46,  33, 64, 62, 91,  89,  70, 104, // 7
         84,  0,  0,  37, 124, 0,   0,  153  // 8
     },
     {
         // Queens - early/mid
         //  A   B   C   D   E   F   G   H
         1,   -10, -11, 3,  -15, -51, -83, -13, // 1
         -7,  3,   2,   5,  -1,  -10, -7,  -2,  // 2
         -11, 0,   12,  2,  8,   11,  7,   -6,  // 3
         -9,  5,   7,   9,  18,  17,  26,  4,   // 4
         -6,  0,   15,  25, 32,  9,   26,  12,  // 5
         -16, 10,  13,  25, 37,  30,  15,  26,  // 6
         1,   11,  35,  0,  16,  55,  39,  57,  // 7
         -13, 6,   -42, 0,  29,  0,   0,   102  // 8
     },
     {
         // Kings - early/mid
         //  A   B   C   D   E   F   G   H
         0,  0,  0,  -9, 0,  -9, 25, 0,  // 1
         -9, -9, -9, -9, -9, -9, -9, -9, // 2
         -9, -9, -9, -9, -9, -9, -9, -9, // 3
         -9, -9, -9, -9, -9, -9, -9, -9, // 4
         -9, -9, -9, -9, -9, -9, -9, -9, // 5
         -9, -9, -9, -9, -9, -9, -9, -9, // 6
         -9, -9, -9, -9, -9, -9, -9, -9, // 7
         -9, -9, -9, -9, -9, -9, -9, -9  // 8
     }},
    {{
         // Pawns - endgame
         //  A   B   C   D   E   F   G   H
         0,   0,   0,   0,   0,   0,   0,   0,   // 1
         -17, -17, -17, -17, -17, -17, -17, -17, // 2
         -11, -11, -11, -11, -11, -11, -11, -11, // 3
         -7,  -7,  -7,  -7,  -7,  -7,  -7,  -7,  // 4
         16,  16,  16,  16,  16,  16,  16,  16,  // 5
         55,  55,  55,  55,  55,  55,  55,  55,  // 6
         82,  82,  82,  82,  82,  82,  82,  82,  // 7
         0,   0,   0,   0,   0,   0,   0,   0    // 8
     },
     {
         // Knights - endgame
         //  A   B   C   D   E   F   G   H
         -99, -99, -94, -88, -88, -94, -99, -99, // 1
         -81, -62, -49, -43, -43, -49, -62, -81, // 2
         -46, -27, -15, -9,  -9,  -15, -27, -46, // 3
         -22, -3,  10,  16,  16,  10,  -3,  -22, // 4
         -7,  12,  25,  31,  31,  25,  12,  -7,  // 5
         -2,  17,  30,  36,  36,  30,  17,  -2,  // 6
         -7,  12,  25,  31,  31,  25,  12,  -7,  // 7
         -21, -3,  10,  16,  16,  10,  -3,  -21  // 8
     },
     {
         // Bishops - endgame
         //  A   B   C   D   E   F   G   H
         -27, -21, -17, -15, -15, -17, -21, -27, // 1
         -10, -4,  0,   2,   2,   0,   -4,  -10, // 2
         2,   8,   12,  14,  14,  12,  8,   2,   // 3
         11,  17,  21,  23,  23,  21,  17,  11,  // 4
         14,  20,  24,  26,  26,  24,  20,  14,  // 5
         13,  19,  23,  25,  25,  23,  19,  13,  // 6
         8,   14,  18,  20,  20,  18,  14,  8,   // 7
         -2,  4,   8,   10,  10,  8,   4,   -2   // 8
     },
     {
         // Rooks - endgame
         //  A   B   C   D   E   F   G   H
         -32, -31, -30, -29, -29, -30, -31, -32, // 1
         -27, -25, -24, -24, -24, -24, -25, -27, // 2
         -15, -13, -12, -12, -12, -12, -13, -15, // 3
         1,   2,   3,   4,   4,   3,   2,   1,   // 4
         15,  17,  18,  18,  18,  18,  17,  15,  // 5
         25,  27,  28,  28,  28,  28,  27,  25,  // 6
         27,  28,  29,  30,  30,  29,  28,  27,  // 7
         16,  17,  18,  19,  19,  18,  17,  16   // 8
     },
     {
         // Queens - endgame
         //  A   B   C   D   E   F   G   H
         -61, -55, -52, -50, -50, -52, -55, -61, // 1
         -31, -26, -22, -21, -21, -22, -26, -31, // 2
         -8,  -3,  1,   3,   3,   1,   -3,  -8,  // 3
         9,   14,  17,  19,  19,  17,  14,  9,   // 4
         19,  24,  28,  30,  30,  28,  24,  19,  // 5
         23,  28,  32,  34,  34,  32,  28,  23,  // 6
         21,  26,  30,  31,  31,  30,  26,  21,  // 7
         12,  17,  21,  23,  23,  21,  17,  12   // 8
     },
     {
         // Kings - endgame
         //  A   B   C   D   E   F   G   H
         -34, -30, -28, -27, -27, -28, -30, -34, // 1
         -17, -13, -11, -10, -10, -11, -13, -17, // 2
         -2,  2,   4,   5,   5,   4,   2,   -2,  // 3
         11,  15,  17,  18,  18,  17,  15,  11,  // 4
         22,  26,  28,  29,  29,  28,  26,  22,  // 5
         31,  34,  37,  38,  38,  37,  34,  31,  // 6
         38,  41,  44,  45,  45,  44,  41,  38,  // 7
         42,  46,  48,  50,  50,  48,  46,  42   // 8
     }}};

int mirror64[64] = {56, 57, 58, 59, 60, 61, 62, 63, 48, 49, 50, 51, 52,
                    53, 54, 55, 40, 41, 42, 43, 44, 45, 46, 47, 32, 33,
                    34, 35, 36, 37, 38, 39, 24, 25, 26, 27, 28, 29, 30,
                    31, 16, 17, 18, 19, 20, 21, 22, 23, 8,  9,  10, 11,
                    12, 13, 14, 15, 0,  1,  2,  3,  4,  5,  6,  7};

int mg_eval(Board *board)
{
        int i, piece, square;
        int v = 0;
        v = board->material[WHITE] - board->material[BLACK];

        piece = wP;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[0][0][SQ120TO64(square)];
        }
        piece = bP;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[0][0][mirror64[SQ120TO64(square)]];
        }

        piece = wN;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[0][1][SQ120TO64(square)];
        }
        piece = bN;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[0][1][mirror64[SQ120TO64(square)]];
        }

        piece = wB;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[0][2][SQ120TO64(square)];
        }
        piece = bB;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[0][2][mirror64[SQ120TO64(square)]];
        }

        piece = wR;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[0][3][SQ120TO64(square)];
        }
        piece = bR;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[0][3][mirror64[SQ120TO64(square)]];
        }

        piece = wQ;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[0][4][SQ120TO64(square)];
        }
        piece = bQ;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[0][4][mirror64[SQ120TO64(square)]];
        }
        piece = wK;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[0][5][SQ120TO64(square)];
        }

        piece = bK;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[0][5][mirror64[SQ120TO64(square)]];
        }

        if (board->turn == WHITE) {
                return v;
        } else {
                return -v;
        }
}

int eg_eval(Board *board)
{
        int i, piece, square;
        int v = 0;
        v = board->material[WHITE] - board->material[BLACK];

        piece = wP;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[1][0][SQ120TO64(square)];
        }
        piece = bP;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[1][0][mirror64[SQ120TO64(square)]];
        }

        piece = wN;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[1][1][SQ120TO64(square)];
        }
        piece = bN;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[1][1][mirror64[SQ120TO64(square)]];
        }

        piece = wB;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[1][2][SQ120TO64(square)];
        }
        piece = bB;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[1][2][mirror64[SQ120TO64(square)]];
        }

        piece = wR;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[1][3][SQ120TO64(square)];
        }
        piece = bR;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[1][3][mirror64[SQ120TO64(square)]];
        }

        piece = wQ;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[1][4][SQ120TO64(square)];
        }
        piece = bQ;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[1][4][mirror64[SQ120TO64(square)]];
        }
        piece = wK;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v += piece_square_table[1][5][SQ120TO64(square)];
        }

        piece = bK;
        for (i = 0; i < board->pieceCount[piece]; i++) {
                square = board->pieceList[piece][i];
                v -= piece_square_table[1][5][mirror64[SQ120TO64(square)]];
        }

        if (board->turn == WHITE) {
                return v;
        } else {
                return -v;
        }
}

int phase(Board *board)
{
        int pawn_phase = 0;
        int knight_phase = 1;
        int bishop_phase = 1;
        int rook_phase = 2;
        int queen_phase = 4;
        int total_phase = pawn_phase * 16 + knight_phase * 4 +
                          bishop_phase * 4 + rook_phase * 4 + queen_phase * 2;
        int phase = total_phase;

        phase -= knight_phase * (board->pieceCount[wN] + board->pieceCount[bN]);
        phase -= bishop_phase * (board->pieceCount[wB] + board->pieceCount[bB]);
        phase -= rook_phase * (board->pieceCount[wR] + board->pieceCount[bR]);
        phase -= queen_phase * (board->pieceCount[wQ] + board->pieceCount[bQ]);

        return (phase * 256 + (total_phase / 2)) / total_phase;
}

int evaluate(Board *board)
{
        int mg = mg_eval(board);
        int eg = eg_eval(board);
        int p = phase(board);
        return ((mg * (256 - p)) + (eg * p)) / 256;
}
