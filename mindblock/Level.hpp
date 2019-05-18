#ifndef SAXBOPHONE_MINDBLOCK_LEVEL_HPP
#define SAXBOPHONE_MINDBLOCK_LEVEL_HPP

#include <unordered_set>
#include <vector>

#include <cstdint>

#include "Block.hpp"
#include "BlockType.hpp"

namespace mindblock {
    enum struct Direction : uint_fast8_t {
        Left,
        Up,
        Right,
        Down,
    };

    class Level {
    public:
        Level(size_t grid_size=7);

        // used to enact the player's moves
        void shift(Direction move);
    private:
        // returns true if it is possible to shift the Blocks in the given direction
        bool shift_possible(Direction move) const;
        // set of all the blocks used in the puzzle
        std::unordered_set<BlockType, hash_block_type> block_palette;
        size_t grid_size; // the size of each grid axis
        std::vector<std::vector<Block*>> grid; // the game grid
        // the actual blocks that the pointers in the game grid point to
        std::vector<Block> blocks;
    };
}

#endif // include guard
