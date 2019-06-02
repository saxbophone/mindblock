#include "mindblock/Game.hpp"


int main() {
    mindblock::Game game;
    while (game.is_running()) {
        game.run();
    }
}
