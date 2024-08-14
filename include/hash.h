#ifndef HASH_H
#define HASH_H

#include "board.h"
#include <stdint.h>

extern uint64_t piece_hash[2][PC][SQ];
extern uint64_t ep_hash[8];
extern uint64_t ca_hash[52];
extern uint64_t stm_hash;

void init_hash();

#endif
