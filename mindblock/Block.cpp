#include "Block.hpp"
#include "Colour.hpp"
#include "Shape.hpp"


namespace mindblock {
    Block::Block(Shape shape, Colour colour)
      : shape(shape), colour(colour)
      {}
}
