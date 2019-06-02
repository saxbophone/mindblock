#ifndef SAXBOPHONE_MINDBLOCK_SHAPE_HPP
#define SAXBOPHONE_MINDBLOCK_SHAPE_HPP

#include <cstdint>


namespace mindblock {
    enum struct Shape : uint_fast8_t {
        Square,
        // NOTE: this is a placeholder only, used for accessing the number of shapes
        META_NUMBER_OF_SHAPES,
    };
}

#endif // include guard
