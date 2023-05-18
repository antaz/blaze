#ifndef ATTACK_H
#define ATTACK_H

#include <stdbool.h>
#include <stdint.h>

#define rank(i) (i >> 3)
#define file(i) (i & 7)
#define diag(i) 7 + rank(i) - file(i)
#define adiag(i) rank(i) + file(i)
#define index(f, r) (8 * r + f)

/** @brief king attacks from square
 *  @param i square index
 *  @return uint64_t
 */
uint64_t katk(const int i);

/** @brief knight attacks from square
 *  @param i square index
 *  @return uint64_t
 */
uint64_t natk(const int i);

/** @brief bishop attacks from square
 *  @param i square index
 *  @param block bitset of blockers
 *  @return uint64_t
 */
uint64_t batk(const int i, const uint64_t block);

/** @brief rook attacks from square
 *  @param i square index
 *  @param block bitset of blockers
 *  @return uint64_t
 */
uint64_t ratk(const int i, const uint64_t block);

/** @brief bitset between two squares
 *  @param i start square index
 *  @param j destination square index
 *  @return uint64_t
 */
uint64_t between(const int i, const int j);

/** @brief bitset of the line between two squares
 *  @param i start square index
 *  @param j destination square index
 *  @return uint64_t
 */
uint64_t line(const int i, const int j);

#endif /* ATTACK_H */
