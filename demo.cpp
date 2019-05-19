#include "mindblock/Level.hpp"


int main(void) {
    using namespace mindblock;
    // test Level class
    Level level;
    level.print();
    // a sequence of moves to make
    Direction moves[] = {
        Direction::Left,
        Direction::Left,
        Direction::Up,
        Direction::Right,
        Direction::Right,
    };
    for (auto move : moves) {
        level.shift(move);
        level.print();
    }
    return 0;
}
