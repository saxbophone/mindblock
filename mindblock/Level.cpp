#include <random>
#include <type_traits>
#include <utility>

#include <cstdio>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include "Level.hpp"


namespace mindblock {
    Level::Level(std::mt19937& random_number_engine, size_t grid_size)
      : random_number_engine(random_number_engine)
      , grid_size(grid_size)
      {
        // set grid vector to the correct size
        this->grid.resize(grid_size);
        // resize vector's columns too
        for (auto &column : this->grid) {
            column.resize(grid_size, NULL);
        }
        // generate a random puzzle
        this->generate_random_puzzle();
        // attach any attachable blocks before starting
        this->attach_blocks();
    }

    bool Level::shift(Direction move) {
        // check move is possible before moving blocks
        if (this->shift_possible(move)) {
            switch (move) {
                case Direction::Left: {
                    // when shifting left, we start at the second column
                    for (size_t x = 1; x < this->grid_size; x++) {
                        for (size_t y = 0; y < this->grid_size; y++) {
                            Block* candidate = this->grid[x][y];
                            if (candidate != NULL && candidate->is_attached) {
                                std::swap(this->grid[x - 1][y], this->grid[x][y]);
                            }
                        }
                    }
                    break;
                }
                case Direction::Right: {
                    // when shifting right, we start at the penultimate column
                    for (size_t x = this->grid_size - 1; x > 0; x--) {
                        for (size_t y = 0; y < this->grid_size; y++) {
                            Block* candidate = this->grid[x - 1][y];
                            if (candidate != NULL && candidate->is_attached) {
                                std::swap(this->grid[x - 1][y], this->grid[x][y]);
                            }
                        }
                    }
                    break;
                }
                case Direction::Up: {
                    // when shifting up, we start at the second row
                    for (size_t x = 0; x < this->grid_size; x++) {
                        for (size_t y = 1; y < this->grid_size; y++) {
                            Block* candidate = this->grid[x][y];
                            if (candidate != NULL && candidate->is_attached) {
                                std::swap(this->grid[x][y - 1], this->grid[x][y]);
                            }
                        }
                    }
                    break;
                }
                case Direction::Down: {
                    // when shifting down, we start at the penultimate row
                    for (size_t x = 0; x < this->grid_size; x++) {
                        for (size_t y = this->grid_size - 1; y > 0; y--) {
                            Block* candidate = this->grid[x][y - 1];
                            if (candidate != NULL && candidate->is_attached) {
                                std::swap(this->grid[x][y - 1], this->grid[x][y]);
                            }
                        }
                    }
                    break;
                }
            }
            // recalculate attached blocks
            this->attach_blocks();
            // a shift was possible, return true to indicate this
            return true;
        } else {
            // indicate that a shift was not possible
            return false;
        }
    }

    void Level::attach_blocks() {
        // iterate over whole grid and check each Block that is attached
        for (size_t x = 0; x < this->grid_size; x++) {
            for (size_t y = 0; y < this->grid_size; y++) {
                Block* block = this->grid[x][y];
                if (block != NULL && block->is_attached) {
                    // XXX: mark block as unattached so the recursive method works
                    block->is_attached = false;
                    this->attach_block(x, y);
                }
            }
        }
    }

    void Level::draw(sf::RenderWindow& window) {
        // get window dimensions
        sf::Vector2u window_size = window.getSize();
        // the standard dimensions of blocks
        sf::Vector2f block_size(
            (double)window_size.x / this->grid_size,
            (double)window_size.y / this->grid_size
        );
        const float border_multiplier = 0.075f;
        // we start off with a larger block size for creating border of attached
        sf::RectangleShape sprite(block_size * (1.0f + border_multiplier));
        // offset the sprite by half of the size increase to create the border
        sprite.setOrigin(block_size * border_multiplier / 2.0f);
        // the border of attached Blocks is white
        sprite.setFillColor(sf::Color::White);
        // check every cell in the grid for a Block
        for (size_t y = 0; y < this->grid_size; y++) {
            for (size_t x = 0; x < this->grid_size; x++) {
                Block* block = this->grid[x][y];
                // we only render attached Blocks on this first pass
                if (block != NULL && block->is_attached) {
                    // set Block position and render it
                    sprite.setPosition(
                        (double)window_size.x / this->grid_size * x,
                        (double)window_size.y / this->grid_size * y
                    );
                    window.draw(sprite);
                }
            }
        }
        // reset the origin
        sprite.setOrigin(0.0f, 0.0f);
        // set the sprite size to the 'standard' size now to render ALL Blocks
        sprite.setSize(block_size);
        for (size_t y = 0; y < this->grid_size; y++) {
            for (size_t x = 0; x < this->grid_size; x++) {
                Block* block = this->grid[x][y];
                if (block != NULL) {
                    // NOTE: in the future, we'll need to check Block Shape
                    BlockType type = *block->block_type;
                    // this dereference prevents COLOUR_MAP from being const for some reason
                    sprite.setFillColor(COLOUR_MAP.at(type.colour));
                    // set Block position and render it
                    sprite.setPosition(
                        (double)window_size.x / this->grid_size * x,
                        (double)window_size.y / this->grid_size * y
                    );
                    window.draw(sprite);
                }
            }
        }
    }

    void Level::print() {
        printf("\n");
        for (size_t x = 0; x < this->grid_size + 2; x++) {
            printf("#");
        }
        printf("\n#");
        for (size_t y = 0; y < this->grid_size; y++) {
            for (size_t x = 0; x < this->grid_size; x++) {
                Block* block = this->grid[x][y];
                if (block == NULL) {
                    printf(" ");
                } else if (block->is_attached) {
                    printf("X");
                } else {
                    printf("O");
                }
            }
            printf("#\n#");
        }
        for (size_t x = 0; x < this->grid_size + 1; x++) {
            printf("#");
        }
        printf("\n");
    }

    void Level::generate_random_puzzle() {
        // these are our PRNGs for Blocks in the puzzle
        std::uniform_int_distribution<std::underlying_type<Colour>::type>
            colour_generator(
                0,
                (std::underlying_type<Colour>::type)
                Colour::META_NUMBER_OF_COLOURS - 1
            );
        std::uniform_int_distribution<std::underlying_type<Shape>::type>
            shape_generator(
                0,
                (std::underlying_type<Shape>::type)
                Shape::META_NUMBER_OF_SHAPES - 1
            );
        std::uniform_int_distribution<size_t>
            co_ord_generator(0, this->grid_size - 1);
        // generate the same number of Blocks as our grid size
        // reserve space in the block palette first of all to guarantee iterator validity
        this->block_palette.reserve(this->grid_size);
        // do the same for the blocks vector
        this->blocks.reserve(this->grid_size);
        for (size_t i = 0; i < this->grid_size; i++) {
            // randomly generate a BlockType and insert into block palette
            BlockType block_type = BlockType(
                (Colour)colour_generator(this->random_number_engine),
                (Shape)shape_generator(this->random_number_engine)
            );
            // this BlockType might already exist but it doesn't matter
            this->block_palette.insert(block_type);
            // create a Block of this type and insert into Blocks vector
            this->blocks.push_back(Block(this->block_palette.find(block_type)));
            // choose a location for this Block (make sure it's unoccupied)
            size_t x, y = 0;
            do {
                x = co_ord_generator(random_number_engine);
                y = co_ord_generator(random_number_engine);
            } while (this->grid[x][y] != NULL); // if exists, pick a new cell
            this->grid[x][y] = &this->blocks[i];
        }
        // finally, pick one random block to be 'attached'
        this->blocks[co_ord_generator(random_number_engine)].is_attached = true;
    }

    void Level::attach_block(size_t x, size_t y) {
        Block* block = this->grid[x][y];
        if (block != NULL && !block->is_attached) {
            // mark the Block as attached
            block->is_attached = true;
            // call this method recursively on adjacent cells
            if (x > 0) { // to the left
                this->attach_block(x - 1, y);
            }
            if (x < this->grid_size - 1) { // to the right
                this->attach_block(x + 1, y);
            }
            if (y > 0) { // to the top
                this->attach_block(x, y - 1);
            }
            if (y < this->grid_size - 1) { // to the bottom
                this->attach_block(x, y + 1);
            }
        }
    }

    bool Level::shift_possible(Direction move) const {
        size_t zero = 0;
        size_t last = this->grid_size - 1;
        size_t* x = &zero; // initially assume we'll check the first column
        size_t* y = &zero; // initially assume we'll check the first row
        size_t i = 0; // this index will be incremented
        // used for specfying the range of attached blocks to shift
        size_t start = 1;
        size_t end = this->grid_size;
        // used for axis-aligned offsets of moves
        int delta = -1;
        switch (move) {
        case Direction::Right:
            // if shifting right, we need to check the last column instead
            x = &last;
            start = 0;
            end = this->grid_size - 1;
            delta = 1;
            // FALLTHROUGH!
        case Direction::Left:
            // as we're moving horizontally, we want to modify the y index
            y = &i;
            break;
        case Direction::Down:
            // if shifting down, we need to check the last row instead
            y = &last;
            start = 0;
            end = this->grid_size - 1;
            delta = 1;
            // FALLTHROUGH!
        case Direction::Up:
            // as we're moving horizontally, we want to modify the x index
            x = &i;
            break;
        }
        for (i = 0; i < this->grid_size; i++) {
            // make sure there are no attached blocks in this range
            Block* block = this->grid[*x][*y];
            if (block != NULL && block->is_attached) {
                return false;
            }
        }
        /*
         * now, check every attached block and make sure the cell we want to
         * move it to does not contain an unattached block
         */
        switch (move) {
        case Direction::Left:
        case Direction::Right:
            for (size_t x = start; x < end; x++) {
                for (size_t y = 0; y < this->grid_size; y++) {
                    if (this->grid[x][y] != NULL && this->grid[x][y]->is_attached) {
                        Block* block = this->grid[x + delta][y];
                        if (block != NULL && !block->is_attached) {
                            return false;
                        }
                    }
                }
            }
            break;
        case Direction::Up:
        case Direction::Down:
            for (size_t x = 0; x < this->grid_size; x++) {
                for (size_t y = start; y < end; y++) {
                    if (this->grid[x][y] != NULL && this->grid[x][y]->is_attached) {
                        Block* block = this->grid[x][y + delta];
                        if (block != NULL && !block->is_attached) {
                            return false;
                        }
                    }
                }
            }
            break;
        }
        // if we got this far, all are empty and the shift is possible
        return true;
    }
}
