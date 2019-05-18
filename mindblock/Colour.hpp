#ifndef SAXBOPHONE_MINDBLOCK_COLOUR_HPP
#define SAXBOPHONE_MINDBLOCK_COLOUR_HPP

#include <cstdint>


namespace mindblock {
    enum struct Colour : uint_fast8_t {
        Grey, // default bland colour
        // the rainbow, but divided mathematically according to light primaries
        Red,
        Orange,
        Yellow,
        Lime,
        Green,
        Teal,
        Cyan,
        Prussian, // XXX: Think of a better name?
        Blue,
        Indigo,
        Purple,
        Violet,
        // end of rainbow
        // other misc. colours
        Brown,
    };
}

#endif // include guard
