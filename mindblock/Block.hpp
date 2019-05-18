#ifndef SAXBOPHONE_MINDBLOCK_BLOCK_HPP
#define SAXBOPHONE_MINDBLOCK_BLOCK_HPP

#include <functional>

#include "Colour.hpp"
#include "Shape.hpp"

namespace mindblock {
    class Block {
    public:
        Block(Shape shape=Shape::Square, Colour colour=Colour::Grey);

        Shape shape;
        Colour colour;
    };

    struct hash_block {
        size_t operator()(const Block &block) const {
            return (
                std::hash<std::underlying_type<Shape>::type>()(
                    (std::underlying_type<Shape>::type)block.shape
                ) ^
                std::hash<std::underlying_type<Colour>::type>()(
                    (std::underlying_type<Colour>::type)block.colour
                )
            );
        }
    };
}

#endif // include guard
