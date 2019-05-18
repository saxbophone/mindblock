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
        // shifting algorithm:
        // - check move is possible
        // - move all 'attached' blocks by shift vector
        // - attach any 'unattached' blocks that are now adjacent to 'attached' ones

        // check move is possible before moving blocks
        if (this->shift_possible(move)) {
            switch (move) {
                case Direction::Left: {
                    // when shifting left, we start at the second column
                    for (size_t x = 1; x < grid_size; x++) {
                        for (size_t y = 0; y < grid_size; y++) {
                            swap(this->grid[x][y], this->grid[x - 1][y]);
                        }
                    }
                    break;
                }
                case Direction::Right: {
                    // when shifting right, we start at the penultimate column
                    // ARGH I DON'T KNOW IF ANY OF THIS IS RIGHT!
                    for (size_t x = grid_size - 1; x > 1; x--) {
                        for (size_t y = 0; y < grid_size; y++) {
                            swap(this->grid[x - 1][y], this->grid[x - 2][y]);
                        }
                    }
                    break;
                }
            }
        }
        // TODO: implement shifting algorithm
        return;
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
}
