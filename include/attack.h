#ifndef ATTACK_H
#define ATTACK_H

#include <stdint.h>

#define rank(i) (i >> 3)
#define file(i) (i & 7)
#define diag(i) 7 + rank(i) - file(i)
#define adiag(i) rank(i) + file(i)
#define index(f, r) (8 * r + f)

uint64_t katk(const int i);

uint64_t wpatk(const int i);

uint64_t bpatk(const int i);

uint64_t natk(const int i);

uint64_t batk(const int i, const uint64_t block);

uint64_t ratk(const int i, const uint64_t block);

uint64_t between(const int i, const int j);

uint64_t line(const int i, const int j);

#endif /* ATTACK_H */
