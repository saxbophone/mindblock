#ifndef SAXBOPHONE_MINDBLOCK_BLOCK_HPP
#define SAXBOPHONE_MINDBLOCK_BLOCK_HPP

#include "Colour.hpp"
#include "Shape.hpp"

namespace mindblock {
    class Block {
    public:
        Block(Colour colour=Colour::Grey, Shape shape=Shape::Square);
    };
}

#endif // include guard
