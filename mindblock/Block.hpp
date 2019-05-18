#ifndef SAXBOPHONE_MINDBLOCK_BLOCK_HPP
#define SAXBOPHONE_MINDBLOCK_BLOCK_HPP

#include "BlockType.hpp"
#include "Shape.hpp"


namespace mindblock {
    class Block {
    public:
        Block(bool is_attached=false, BlockType* block_type=NULL);

        bool is_attached; // is this Block attached to the mass of Blocks?
        BlockType* block_type; // what type (shape and colour) this Block is
    };
}

#endif // include guard
