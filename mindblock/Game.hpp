#ifndef SAXBOPHONE_MINDBLOCK_GAME_HPP
#define SAXBOPHONE_MINDBLOCK_GAME_HPP

#include <random>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

#include "Level.hpp"


namespace mindblock {
    class Game {
    public:
        Game();
        // the main game loop method
        void run();
        // is the Game still running?
        bool is_running() const;
    private:
        std::mt19937 random_number_engine;
        Level current_level;
        sf::RenderWindow window;
        bool running;
        // mouse button object-drag state
        bool mouse_button_held;
        sf::Vector2<double> touch_start;
        // responds to events
        void handle_events();
        // draws the Game to the window
        void draw();
    };
}

#endif // include guard
