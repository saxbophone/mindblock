#ifndef SAXBOPHONE_MINDBLOCK_LEVEL_HPP
#define SAXBOPHONE_MINDBLOCK_LEVEL_HPP

#include <unordered_set>

#include "Block.hpp"

namespace mindblock {
    class Level {
    public:
        Level();
    private:
        std::unordered_set<Block, hash_block> block_palette;
    };
}

#endif // include guard
