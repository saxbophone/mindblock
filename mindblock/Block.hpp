#ifndef SAXBOPHONE_MINDBLOCK_BLOCK_HPP
#define SAXBOPHONE_MINDBLOCK_BLOCK_HPP

#include "Colour.hpp"
#include "Shape.hpp"

namespace mindblock {
    class Block {
    public:
        Block(Shape shape=Shape::Square, Colour colour=Colour::Grey);

        Shape shape;
        Colour colour;
    };

    /*
     * this hashing structure is needed so we can stuff Block instances into an
     * unordered_set
     */
    struct hash_block {
        size_t operator()(const Block &block) const;
    };
}

#endif // include guard
