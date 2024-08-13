#ifndef HASH_H
#define HASH_H

#include <stdint.h>

extern uint64_t piece_hash[2][6][64];
extern uint64_t ep_bash[8];
extern uint64_t ca_hash[16];
extern uint64_t turn_hash;

void init_hash();

#endif
