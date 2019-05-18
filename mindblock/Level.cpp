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
}
