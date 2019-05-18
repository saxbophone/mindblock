#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "mindblock/Block.hpp"


int main(void) {
    // this is a demo program for testing keypress-grabbing
    sf::RenderWindow window;
    while(true) {
        sf::Event event;
        while(window.pollEvent(event)) {
            // look out for KeyReleased events
            if (event.type == sf::Event::KeyReleased) {
                // find out which key and act accordingly
                switch (event.key.code) {
                case sf::Keyboard::Key::X:
                    // quit!
                    break;
                default:
                    // continue!
                    break;
                }
            }
        }
    }
    return 0;
}
