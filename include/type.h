#ifndef TYPE_H
#define TYPE_H

/* number of squares on the board */
#define SQ 64

/* number of piece types */
#define PC 6

/* maximum number of moves */
#define MM 256

/* no square, used for empty ep square */
#define NOSQ 8

/* colors and turns */
enum { WHITE = 0, BLACK = 8 };

/* pieces */
enum { NOPC, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

/* move types */
enum {
    QUIET,   // Quiet move
    DPP,     // Double pawn push
    OO,      // King side castle
    OOO,     // Queen side castle
    CAPTURE, // Capture
    EP,      // En-Passent
    NP,      // Knight promotion
    BP,      // Bishop promotion
    RP,      // Rook promotion
    QP,      // Queen promotion
    NPC,     // Knight promotion capture
    BPC,     // Bishop promotion capture
    RPC,     // Rook promotion capture
    QPC      // Queen promotion capture
};

/* castling permissions */
enum {
    WK = 0x02, // White king side
    WQ = 0x01, // White queen side
    BK = 0x20, // Black king side
    BQ = 0x10  // Black queen side
};

// move serializers and deserializers
#define MOVE(from, to, type)                                                   \
    (type & 0xf) << 12 | (from & 0x3f) << 6 | (to & 0x3f)
#define MOVE_FROM(move) ((move >> 6) & 0x3f)
#define MOVE_TO(move) (move & 0x3f)
#define MOVE_TYPE(move) ((move >> 12) & 0x0f)

#endif /* TYPE_H */
