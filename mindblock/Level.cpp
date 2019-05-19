#include <utility>

#include "Level.hpp"


namespace mindblock {
    Level::Level(size_t grid_size)
      : grid_size(grid_size)
      {
        // set grid vector to the correct size
        this->grid.resize(grid_size);
        // resize vector's columns too
        for (auto &column : this->grid) {
            column.resize(grid_size, NULL);
        }
        // TODO: randomly generate a puzzle
    }

    void Level::shift(Direction move) {
        // check move is possible before moving blocks
        if (this->shift_possible(move)) {
            switch (move) {
                case Direction::Left: {
                    // when shifting left, we start at the second column
                    for (size_t x = 1; x < grid_size; x++) {
                        for (size_t y = 0; y < grid_size; y++) {
                            Block* candidate = this->grid[x][y];
                            if (candidate->is_attached) {
                                std::swap(this->grid[x - 1][y], candidate);
                            }
                        }
                    }
                    break;
                }
                case Direction::Right: {
                    // when shifting right, we start at the penultimate column
                    for (size_t x = grid_size - 1; x > 1; x--) {
                        for (size_t y = 0; y < grid_size; y++) {
                            Block* candidate = this->grid[x][y];
                            if (candidate->is_attached) {
                                std::swap(this->grid[x - 1][y], candidate);
                            }
                        }
                    }
                    break;
                }
                case Direction::Up: {
                    // when shifting up, we start at the second row
                    for (size_t x = 0; x < grid_size; x++) {
                        for (size_t y = 1; y < grid_size; y++) {
                            Block* candidate = this->grid[x][y];
                            if (candidate->is_attached) {
                                std::swap(this->grid[x][y - 1], candidate);
                            }
                        }
                    }
                    break;
                }
                case Direction::Down: {
                    // when shifting down, we start at the penultimate row
                    for (size_t x = 0; x < grid_size; x++) {
                        for (size_t y = grid_size - 1; y > 1; y--) {
                            Block* candidate = this->grid[x][y];
                            if (candidate->is_attached) {
                                std::swap(this->grid[x][y - 1], candidate);
                            }
                        }
                    }
                    break;
                }
            }
            // recalculate attached blocks
            this->attach_blocks();
        }
    }

    void Level::attach_blocks() {
        // iterate over whole grid and check each Block that is attached
        for (size_t x = 0; x < grid_size; x++) {
            for (size_t y = 0; y < grid_size; y++) {
                Block* block = this->grid[x][y];
                if (block != NULL && block->is_attached) {
                    // XXX: mark block as unattached so the recursive method works
                    block->is_attached = false;
                    this->attach_block(x, y);
                }
            }
        }
    }

    bool Level::shift_possible(Direction move) const {
        size_t zero = 0;
        size_t last = grid_size - 1;
        size_t* x = &zero; // initially assume we'll check the first column
        size_t* y = &zero; // initially assume we'll check the first row
        size_t i = 0; // this index will be incremented
        switch (move) {
        case Direction::Right:
            // if shifting right, we need to check the last column instead
            x = &last;
            // FALLTHROUGH!
        case Direction::Left:
            // as we're moving horizontally, we want to modify the y index
            y = &i;
            break;
        case Direction::Down:
            // if shifting down, we need to check the last row instead
            y = &last;
            // FALLTHROUGH!
        case Direction::Up:
            // as we're moving horizontally, we want to modify the x index
            x = &i;
            break;
        }
        for (i = 0; i < grid_size; i++) {
            // check each cell in this range to make sure it's empty
            if (this->grid[*x][*y] != NULL) {
                return false;
            }
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
            if (x < grid_size - 1) { // to the right
                this->attach_block(x + 1, y);
            }
            if (y > 0) { // to the top
                this->attach_block(x, y - 1);
            }
            if (y < grid_size - 1) { // to the bottom
                this->attach_block(x, y + 1);
            }
        }
    }
}
