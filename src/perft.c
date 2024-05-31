#include "perft.h"
#include "gen.h"
#include <stdint.h>
#include <stdlib.h>

uint64_t perft(struct board_t *board, int depth)
{
        uint16_t quiets[256];
        uint16_t noisies[256];
        int count;
        uint64_t nodes = 0;

        if (depth == 0)
                return 1ULL;

        // generate captures
        count = quiet(board, quiets);
        count += noisy(board, noisies);

        for (int i = 0; i < count; ++i) {
                nodes += perft(board, depth - 1);
        }

        return nodes;
}
