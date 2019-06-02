#include <functional>

#include <cstdint>

#include "BlockType.hpp"
#include "Colour.hpp"
#include "Shape.hpp"


namespace mindblock {
    BlockType::BlockType(Colour colour, Shape shape)
      : shape(shape), colour(colour)
      {}

    bool BlockType::operator==(const BlockType &other) const {
        return other.colour == this->colour && other.shape == this->shape;
    }

    size_t hash_block_type::operator()(const BlockType &block_type) const {
        // XXX: implementation-dependent, relies on uint8_t types for members
        return std::hash<uint_fast16_t>()(
            ((uint_fast16_t)block_type.shape << 8) &
            (uint_fast16_t)block_type.colour
        );
    }
}
