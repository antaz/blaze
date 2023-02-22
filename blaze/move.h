#ifndef MOVE_H
#define MOVE_H

#define MAXDEPTH 64

struct move_t {
        unsigned char from;      // move from
        unsigned char to;        // move destination
        unsigned char captured;  // captured piece
        unsigned char promoted;  // promoted piece
        unsigned char enPassant; // en-passant flag
        unsigned char pawnStart; // pawn start flag
        unsigned char castle;    // castling flag
        int score;
};


// principal variation  structure
typedef struct {
        struct move_t moves[MAXDEPTH];
        int count;
} PV;

void printMove(struct move_t move);

#endif /* MOVE_H */
