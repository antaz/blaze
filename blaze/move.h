#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>

enum {
        QUIET,       // Quiet move
        DPP,         // Double pawn push
        OO,          // King side castle
        OOO,         // Queen side castle
        CAPTURE,     // Capture
        EP,          // En-Passent
        NP,          // Knight promotion
        BP,          // Bishop promotion
        RP,          // Rook promotion
        QP,          // Queen promotion
        NPC,         // Knight promotion capture 
        BPC,         // Bishop promotion capture
        RPC,         // Rook promotion capture
        QPC          // Queen promotion capture
};

#define from(m) (m >> 6) & 0x3f
#define to(m) (m & 0x3f)
#define flags(m) (m >> 12) & 0x0f

/* construct move */
uint16_t move(int from, int to, int flags);

/* print move */
void printm(uint16_t m);

#endif /* MOVE_H */
