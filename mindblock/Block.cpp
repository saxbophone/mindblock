#include <functional>

#include <cstdint>

#include "Block.hpp"
#include "Colour.hpp"
#include "Shape.hpp"


namespace mindblock {
    Block::Block(Shape shape, Colour colour)
      : shape(shape), colour(colour)
      {}

    size_t hash_block::operator()(const Block &block) const {
        // XXX: implementation-dependent, relies on uint8_t types for members
        return std::hash<uint_fast16_t>()(
            ((uint_fast16_t)block.shape << 8) & (uint_fast16_t)block.colour
        );
    }
}
