#ifndef SAXBOPHONE_MINDBLOCK_LEVEL_HPP
#define SAXBOPHONE_MINDBLOCK_LEVEL_HPP

#include <unordered_set>
#include <vector>

#include "Block.hpp"

namespace mindblock {
    class Level {
    public:
        Level(size_t grid_size=7);
    private:
        // set of all the blocks used in the puzzle
        std::unordered_set<Block, hash_block> block_palette;
        size_t grid_size; // the size of each grid axis
        std::vector<std::vector<Block*>> grid; // the game grid
    };
}

#endif // include guard
