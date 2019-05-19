#include <iostream>

#include "mindblock/Level.hpp"


int main(void) {
    using namespace mindblock;
    std::cout << "Use w/a/s/d to move (followed by enter)" << std::endl;
    // test Level class
    Level level;
    level.attach_blocks();
    level.print();
    while (true) {
        char input = '\0';
        std::cin >> input;
        Direction move;
        switch (input) {
        case 'w':
            move = Direction::Up;
            break;
        case 'a':
            move = Direction::Left;
            break;
        case 's':
            move = Direction::Down;
            break;
        case 'd':
            move = Direction::Right;
            break;
        default:
            continue;
        }
        level.shift(move);
        level.print();
    }
    return 0;
}
