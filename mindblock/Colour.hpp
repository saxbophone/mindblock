#ifndef SAXBOPHONE_MINDBLOCK_COLOUR_HPP
#define SAXBOPHONE_MINDBLOCK_COLOUR_HPP

#include <map>

#include <cstdint>

#include <SFML/Graphics/Color.hpp>


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
        Blue,
        Indigo,
        Purple,
        Magenta,
        Rose,
        // end of rainbow
        // other misc. colours
        Brown,
    };

    // the RGB values for all the colours in our palette
    const std::map<Colour, sf::Color> COLOUR_MAP {
        { Colour::Grey,    sf::Color(127, 127, 127), },
        { Colour::Red,     sf::Color(255,   0,   0), },
        { Colour::Orange,  sf::Color(255, 127,   0), },
        { Colour::Yellow,  sf::Color(255, 255,   0), },
        { Colour::Lime,    sf::Color(128, 255,   0), },
        { Colour::Green,   sf::Color(  0, 255,   0), },
        { Colour::Teal,    sf::Color(  0, 255, 127), },
        { Colour::Cyan,    sf::Color(  0, 255, 255), },
        { Colour::Blue,    sf::Color(  0, 128, 255), },
        { Colour::Indigo,  sf::Color(  0,   0, 255), },
        { Colour::Purple,  sf::Color(127,   0, 255), },
        { Colour::Magenta, sf::Color(255,   0, 255), },
        { Colour::Rose,    sf::Color(255,   0, 127), },
        { Colour::Brown,   sf::Color( 82,  32,   0), },
    };
}

#endif // include guard
