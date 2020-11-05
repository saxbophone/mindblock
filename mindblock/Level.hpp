#ifndef SAXBOPHONE_MINDBLOCK_LEVEL_HPP
#define SAXBOPHONE_MINDBLOCK_LEVEL_HPP

#include <random>
#include <unordered_set>
#include <vector>

#include <cstdint>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Block.hpp"
#include "BlockType.hpp"


namespace mindblock {
    enum struct Direction : uint_fast8_t {
        Left,
        Up,
        Right,
        Down,
    };

    class Level {
    public:
        Level(std::mt19937& random_number_engine, size_t grid_size=7);

        // used to enact the player's moves --returns true if anything changed
        bool shift(Direction move);
        // attaches all Blocks adjacent to attached Blocks
        void attach_blocks();
        // draws the Level state to the given RenderWindow
        void draw(sf::RenderWindow& window);
        // for debugging, prints a bitmap of which cells are occupied/attached
        void print();
        // returns count of attached blocks in the Level
        size_t attached_blocks_count() const;
    private:
        // generates a new random puzzle
        void generate_random_puzzle();
        // tries to generate a new random solution for this given puzzle
        bool generate_random_solution();
        // attaches blocks adjacent to a given Block
        void attach_block(size_t x, size_t y);
        // returns true if it is possible to shift the Blocks in the given direction
        bool shift_possible(Direction move) const;
        // reference to a mersenne twister used for level generation
        std::mt19937& random_number_engine;
        // set of all the blocks used in the puzzle
        std::unordered_set<BlockType, hash_block_type> block_palette;
        size_t grid_size; // the size of each grid axis
        std::vector<std::vector<Block*>> grid; // the game grid
        // the actual blocks that the pointers in the game grid point to
        std::vector<Block> blocks;
    };
}

#endif // include guard
