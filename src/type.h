#ifndef TYPE_H
#define TYPE_H

/* number of squares on the board */
#define SQ 64

/* number of piece types */
#define PC 6

/* maximum number of moves */
#define MM 256

/* no square, used for empty ep square */
#define NOSQ 65

/* colors and turns */
enum { WHITE, BLACK };

/* pieces */
enum { PAWN = 2, KNIGHT, BISHOP, ROOK, QUEEN, KING, NOPC };

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
        WK = 1, // White king side
        WQ = 2, // Black king side
        BK = 4, // White queen side
        BQ = 8  // Black queen side
};

/* piece - piece encoding
   c: piece color
   pt: piece type
 */
#define piece(c, pt) (c >> 3) + pt

/* move - move encoding
   f: from square
   t: to square
   mt: move type
 */
#define move(f, t, mt) (mt & 0xf) << 12 | (f & 0x3f) << 6 | (t & 0x3f)

#endif /* TYPE_H */
