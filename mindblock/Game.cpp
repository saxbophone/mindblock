#include <random>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include "Game.hpp"


namespace mindblock {
    Game::Game()
      : random_number_engine(std::mt19937(std::random_device()()))
      , current_level(this->random_number_engine)
      , window(
            sf::VideoMode(640, 640),
            "Mindblock [DEMO] (c) Joshua Saxby 2019",
            sf::Style::Titlebar | sf::Style::Close
        )
      , running(true)
      {
        // set the game framelimit to something basic and simple
        this->window.setFramerateLimit(30);
    }

    void Game::run() {
        // poll for events
        this->handle_events();
        // draw if still running
        if (this->running) {
            this->draw();
        }
    }

    bool Game::is_running() const {
        return this->running;
    }

    void Game::handle_events() {
        sf::Event event;
        while (this->window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                this->running = false;
                this->window.close();
            // TODO: additional event-handling goes here
            default:
                break;
            }
        }
    }

    void Game::draw() {
        // clear screen to black
        this->window.clear(sf::Color::Black);
        // tell the level to render itself
        this->current_level.draw(this->window);
        // render the window
        this->window.display();
    }
}
