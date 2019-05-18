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
}

#endif // include guard
