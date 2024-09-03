#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

/* move types */
enum {
    QUIET,   // Quiet move
    DPP,     // Double pawn push
    OO,      // King side castle
    OOO,     // Queen side castle
    CAPTURE, // Non special capture
    EP,      // En-Passent capture
    NP = 8,  // Knight promotion
    BP,      // Bishop promotion
    RP,      // Rook promotion
    QP,      // Queen promotion
    NPC,     // Knight promotion capture
    BPC,     // Bishop promotion capture
    RPC,     // Rook promotion capture
    QPC      // Queen promotion capture
};

// Extended move structure
struct move_t {
    uint16_t data; // raw encoding of move
    int score;     // score associated with move
};

// move serializers and deserializers
#define MOVE(from, to, type)                                                   \
    (type & 0xf) << 12 | (from & 0x3f) << 6 | (to & 0x3f)
#define MOVE_FROM(move) ((move >> 6) & 0x3f)
#define MOVE_TO(move) (move & 0x3f)
#define MOVE_TYPE(move) ((move >> 12) & 0x0f)

char *str_move(uint16_t move, int turn);
uint16_t parse_move(char *san);

#endif
