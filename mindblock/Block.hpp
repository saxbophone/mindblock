#ifndef SAXBOPHONE_MINDBLOCK_BLOCK_HPP
#define SAXBOPHONE_MINDBLOCK_BLOCK_HPP

#include <unordered_set>

#include "BlockType.hpp"
#include "Shape.hpp"


namespace mindblock {
    class Block {
    public:
        Block(
            std::unordered_set<BlockType, hash_block_type>::iterator block_type,
            bool is_attached=false
        );

        bool is_attached; // is this Block attached to the mass of Blocks?
        std::unordered_set<BlockType, hash_block_type>::iterator block_type; // what type (shape and colour) this Block is
    };
}

#endif // include guard
