#include <iostream>
#include <random>

#include "mindblock/Level.hpp"


int main(void) {
    namespace mb = mindblock;
    std::cout << "Use w/a/s/d to move (followed by enter)" << std::endl;
    // init PRNG --NOTE: std::random_device() doesn't work on MinGW
    std::mt19937 random_number_engine((std::random_device()()));
    // test Level class
    mb::Level level(random_number_engine);
    level.print();
    while (true) {
        char input = '\0';
        std::cin >> input;
        mb::Direction move;
        switch (input) {
        case 'w':
        case 'W':
            move = mb::Direction::Up;
            break;
        case 'a':
        case 'A':
            move = mb::Direction::Left;
            break;
        case 's':
        case 'S':
            move = mb::Direction::Down;
            break;
        case 'd':
        case 'D':
            move = mb::Direction::Right;
            break;
        default:
            continue;
        }
        if (level.shift(move)) {
            level.print();
        }
    }
    return 0;
}
