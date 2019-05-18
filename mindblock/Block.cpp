#include <functional>

#include <cstdint>

#include "Block.hpp"
#include "BlockType.hpp"


namespace mindblock {
    Block::Block(bool is_attached, BlockType* block_type)
      : is_attached(is_attached), block_type(block_type)
      {}
}
