#ifndef BOARD_H
#define BOARD_H

#include "move.h"
#include "movegen.h"

#define SQNUM 120 // Number of squares on the board
#define U64 unsigned long long
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


// Macro functions
#define FR2SQ64(f, r) (((r) << 3) | (f))
#define FR2SQ120(f, r) (((r) + 2) * 10 + (f) + 1)
#define RANK(sq) ((sq) >> 3)
#define FILE(sq) ((sq)&7)
#define RANK120(sq) ((sq) / 10 - 2)
#define FILE120(sq) ((sq) % 10 - 1)
#define SQ64TO120(sq) ((((sq) >> 3) + 2) * 10 + ((sq)&7) + 1)
#define SQ120TO64(sq) (FR2SQ64(FILE120(sq), RANK120(sq)))
#define isPawn(piece) (piece == 1 || piece == 7)
#define isKnight(piece) (piece == 2 || piece == 8)
#define isBishop(piece) (piece == 3 || piece == 9)
#define isRook(piece) (piece == 4 || piece == 10)
#define isQueen(piece) (piece == 5 || piece == 11)
#define isKing(piece) (piece == 6 || piece == 12)
#define pieceColor(piece) ((piece > 0 && piece <= 6) ? 0 : 1)
#define isSlidePiece(piece) (isBishop(piece) | isRook(piece) | isQueen(piece))
#define onBoard(square)                                                        \
        (square > 20 && square < 99 && square % 10 > 0 && square % 10 < 9 ? 1  \
                                                                          : 0)
#define pieceValid(piece) (piece > 0 && piece < 13 ? 1 : 0)
#define uci_print(...)                                                         \
        ;                                                                      \
        printf(__VA_ARGS__);                                                   \
        fflush(stdout);

// files
enum {
        FILE_A,
        FILE_B,
        FILE_C,
        FILE_D,
        FILE_E,
        FILE_F,
        FILE_G,
        FILE_H,
        FILE_NONE
};

// ranks
enum {
        RANK_1,
        RANK_2,
        RANK_3,
        RANK_4,
        RANK_5,
        RANK_6,
        RANK_7,
        RANK_8,
        RANK_NONE
};
// colors
enum { WHITE, BLACK, BOTH };

// square numbers
enum {
        A1 = 21, B1, C1, D1, E1, F1, G1, H1,
        A2 = 31, B2, C2, D2, E2, F2, G2, H2,
        A3 = 41, B3, C3, D3, E3, F3, G3, H3,
        A4 = 51, B4, C4, D4, E4, F4, G4, H4,
        A5 = 61, B5, C5, D5, E5, F5, G5, H5,
        A6 = 71, B6, C6, D6, E6, F6, G6, H6,
        A7 = 81, B7, C7, D7, E7, F7, G7, H7,
        A8 = 91, B8, C8, D8, E8, F8, G8, H8,
        NO_SQ,
        OFFBOARD
};

// Castling permission constants
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

// TT entry flags
enum { HFNONE, HFALPHA, HFBETA, HFEXACT };

// The undo move sturcture
typedef struct {
        struct move_t move;
        int castling, enPassant, fiftyMove;
        U64 zobristHash;
} Undo;

extern U64 pieceHash[13][120];
extern U64 turnHash;
extern U64 castleHash[16];

/* main structure to hold the state of the board */
struct board_t {
        int pieces[SQNUM];
        int pieceCount[13];
        int pieceList[13][10];
        int kingSquare[2];
        int turn, enPassant, fiftyMove;
        int ply, hisPly, castling;
        int material[2];
        Undo history[MAX];

        int his[13][SQNUM];
        struct move_t kill[2][MAXDEPTH];

        U64 zobristHash;
};

void parseFEN(char *fen, struct board_t *board);
void clearBoard(struct board_t *board);
void updatePieceList(struct board_t *board);
void printBoard(struct board_t *board);
void printPV(PV *pv);
int parseMove(struct board_t *board, char *move, struct move_t *m);
int compareMoves(const void *m1, const void *m2);
int isAttacked(struct board_t *board, int square, int turn);
void addMove(struct board_t *board, MoveList *list, unsigned char from, unsigned char to,
             unsigned char captured, unsigned char promoted,
             unsigned char enPassant, unsigned char pawnStart,
             unsigned char castle);
void addPawnMove(struct board_t *board, MoveList *list, unsigned char from,
                 unsigned char to, unsigned char captured,
                 unsigned char enPassant, unsigned char pawnStart,
                 unsigned char castle);
void generateMoves(struct board_t *board, MoveList *list);
void generateCaptures(struct board_t *board, MoveList *list);

#endif /* BOARD_H */
