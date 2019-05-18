#ifndef SAXBOPHONE_MINDBLOCK_BLOCK_TYPE_HPP
#define SAXBOPHONE_MINDBLOCK_BLOCK_TYPE_HPP

#include "Colour.hpp"
#include "Shape.hpp"


namespace mindblock {
    class BlockType {
    public:
        BlockType(Shape shape=Shape::Square, Colour colour=Colour::Grey);

        Shape shape;
        Colour colour;
    };

    /*
     * this hashing structure is needed so we can stuff BlockType instances
     * into an unordered_set
     */
    struct hash_block_type {
        size_t operator()(const BlockType &block_type) const;
    };
}

#endif // include guard
