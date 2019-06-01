#include "mindblock/Game.hpp"


int main() {
    namespace mb = mindblock;
    mb::Game game;
    while (game.is_running()) {
        game.run();
    }
}
