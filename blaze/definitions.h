#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// debuging and tracing errors

#ifndef NDEBUG
#include <assert.h>
#define ASS(x) assert(x)
#else
#define ASS(x)
#endif

// A couple of constants for our chess engine

#define NAME "Trappist"
#define AUTHOR "Antar Azri"
#define SQNUM 120 // Number of squares on the board
#define U64 unsigned long long
#define MAX 256
#define MAXDEPTH 64
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

// pieces
enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };

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
        A1 = 21,
        B1,
        C1,
        D1,
        E1,
        F1,
        G1,
        H1,
        A2 = 31,
        B2,
        C2,
        D2,
        E2,
        F2,
        G2,
        H2,
        A3 = 41,
        B3,
        C3,
        D3,
        E3,
        F3,
        G3,
        H3,
        A4 = 51,
        B4,
        C4,
        D4,
        E4,
        F4,
        G4,
        H4,
        A5 = 61,
        B5,
        C5,
        D5,
        E5,
        F5,
        G5,
        H5,
        A6 = 71,
        B6,
        C6,
        D6,
        E6,
        F6,
        G6,
        H6,
        A7 = 81,
        B7,
        C7,
        D7,
        E7,
        F7,
        G7,
        H7,
        A8 = 91,
        B8,
        C8,
        D8,
        E8,
        F8,
        G8,
        H8,
        NO_SQ,
        OFFBOARD
};

// Castling permission constants
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

// TT entry flags
enum { HFNONE, HFALPHA, HFBETA, HFEXACT };

typedef struct {
        unsigned char from;      // move from
        unsigned char to;        // move destination
        unsigned char captured;  // captured piece
        unsigned char promoted;  // promoted piece
        unsigned char enPassant; // en-passant flag
        unsigned char pawnStart; // pawn start flag
        unsigned char castle;    // castling flag
        int score;
} Move;

// The undo move sturcture
typedef struct {
        Move move;
        int castling, enPassant, fiftyMove;
        U64 zobristHash;
} Undo;

// Transposition Table entry
typedef struct {
        Move move;
        int score, depth, flags;
        U64 zobristHash;
} TTEntry;

// Transposition table
typedef struct {
        TTEntry *table;
        int newWrite, overWrite, hit, cut;
        U64 size;
} TTable;

// The main board structure for our chess engine
typedef struct {
        int pieces[SQNUM];
        int pieceCount[13];    // The count of pieces on the board
        int pieceList[13][10]; // The squares of where pieces are
        int kingSquare[2];
        int turn, enPassant, fiftyMove;
        int ply, hisPly, castling;
        int material[2];
        Undo history[MAX];

        int his[13][SQNUM];
        Move kill[2][MAXDEPTH];

        U64 zobristHash;
        TTable table[1];

} Board;

// search driver
typedef struct {
        long long starttime, stoptime;
        int depth, timeset, movestogo, infinite, stop;
        long nodes;
} Search;

// Move list sructure and store the generated moves
typedef struct {
        Move moves[MAX];
        int count;
} MoveList;

// principal variation  structure
typedef struct {
        Move moves[MAXDEPTH];
        int count;
} PV;

extern U64 pieceHash[13][120];
extern U64 turnHash;
extern U64 castleHash[16];
#endif
