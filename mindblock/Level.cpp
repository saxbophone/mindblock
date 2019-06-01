#include <random>
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
        // XXX: for now, just generate a fixed puzzle
        this->block_palette.insert(BlockType());
        this->blocks.push_back(Block(this->block_palette.begin(), true));
        this->blocks.push_back(Block(this->block_palette.begin(), false));
        this->blocks.push_back(Block(this->block_palette.begin(), false));
        this->blocks.push_back(Block(this->block_palette.begin(), false));
        this->blocks.push_back(Block(this->block_palette.begin(), false));
        this->blocks.push_back(Block(this->block_palette.begin(), false));
        this->grid[4][3] = &this->blocks[0];
        this->grid[1][2] = &this->blocks[1];
        this->grid[5][5] = &this->blocks[2];
        this->grid[6][0] = &this->blocks[3];
        this->grid[4][4] = &this->blocks[4];
        this->grid[3][6] = &this->blocks[5];
        // TODO: randomly generate a puzzle
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
        // build a block rectangle primitive of the correct dimensions
        sf::RectangleShape sprite(
            sf::Vector2f(
                (double)window_size.x / this->grid_size,
                (double)window_size.y / this->grid_size
            )
        );
        // check every cell in the grid for a Block
        for (size_t y = 0; y < this->grid_size; y++) {
            for (size_t x = 0; x < this->grid_size; x++) {
                Block* block = this->grid[x][y];
                if (block != NULL) { // block exists, get its colour
                    // NOTE: in the future, we'll need to check Block Shape
                    BlockType type = *block->block_type;
                    // this dereference prevents COLOUR_MAP from being const for some reason
                    sprite.setFillColor(COLOUR_MAP.at(type.colour));
                    // set Block position and render it
                    sprite.setPosition(
                        (double)window_size.x / this->grid_size * x,
                        (double)window_size.y / this->grid_size * y
                    );
                    // if Block is attached, give it an outline
                    if (block->is_attached) {
                        // give the shape a negative outline thickness so it extends into itself
                        sprite.setOutlineThickness(
                            // thickness is one 50th of size
                            -(double)window_size.x / this->grid_size / 50
                        );
                    } else {
                        sprite.setOutlineThickness(0.0f);
                    }
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
}
