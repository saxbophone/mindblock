#include <random>

#include <cmath>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/ContextSettings.hpp>
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
            sf::Style::Titlebar | sf::Style::Close // ,
            // sf::ContextSettings(
            //     0, // depth
            //     0, // stencil
            //     1
            // )
        )
      , running(true)
      , mouse_button_held(false)
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
        // any movement to make
        Direction move;
        bool move_made = false;
        while (this->window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                this->running = false;
                this->window.close();
            // XXX: only action move when key released!
            case sf::Event::KeyReleased:
                switch (event.key.code) {
                case sf::Keyboard::Key::Up:
                case sf::Keyboard::Key::W:
                    move = Direction::Up;
                    move_made = true;
                    break;
                case sf::Keyboard::Key::Left:
                case sf::Keyboard::Key::A:
                    move = Direction::Left;
                    move_made = true;
                    break;
                case sf::Keyboard::Key::Down:
                case sf::Keyboard::Key::S:
                    move = Direction::Down;
                    move_made = true;
                    break;
                case sf::Keyboard::Key::Right:
                case sf::Keyboard::Key::D:
                    move = Direction::Right;
                    move_made = true;
                    break;
                default: // XXX: tell C++ we don't care about all other keys
                    break;
                }
                if (move_made) {
                    this->current_level.shift(move);
                }
                break;
            // NOTE: for simplicity/flexibility, we respond to all mouse buttons
            case sf::Event::MouseButtonPressed:
                // just note that the button was pressed and where it was
                this->mouse_button_held = true;
                this->touch_start = sf::Vector2<double>(
                    event.mouseButton.x, event.mouseButton.y
                );
                break;
            case sf::Event::MouseButtonReleased:
                if (this->mouse_button_held) { // only respond if button held
                    // "unhold" the button
                    this->mouse_button_held = false;
                    // compare where mouse is now with where it was when touch started
                    sf::Vector2<double> touch_end(
                        event.mouseButton.x, event.mouseButton.y
                    );
                    sf::Vector2<double> touch_delta = touch_end - touch_start;
                    /*
                     * use pythagoras' theorem to calculate the distance
                     * if distance is the side length of a Block or greater
                     */
                    if (
                        std::sqrt(
                            touch_delta.x * touch_delta.x +
                            touch_delta.y * touch_delta.y
                            // XXX: Hardcoded grid size! Replace 7 later...
                        ) >= this->window.getSize().x / 7.0
                    ) {
                        // scale the delta
                        touch_delta.x /= (this->window.getSize().x / 7.0);
                        touch_delta.y /= (this->window.getSize().x / 7.0);
                        // work out which direction this drag was in and shift
                        if (touch_delta.x <= -1) {
                            move = Direction::Left;
                        } else if (touch_delta.x >= 1) {
                            move = Direction::Right;
                        } else if (touch_delta.y <= -1) {
                            move = Direction::Up;
                        } else if (touch_delta.y >= 1) {
                            move = Direction::Down;
                        }
                        move_made = true;
                    }
                }
                if (move_made) {
                    this->current_level.shift(move);
                }
                break;
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
