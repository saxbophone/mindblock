#include <unordered_set>

#include <cstdint>

#include "Block.hpp"
#include "BlockType.hpp"


namespace mindblock {
    Block::Block(
        std::unordered_set<BlockType, hash_block_type>::iterator block_type,
        bool is_attached
    )
      : is_attached(is_attached)
      , block_type(block_type)
      {}
}
