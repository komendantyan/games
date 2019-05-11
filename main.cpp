#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main() {
    sf::Window window(sf::VideoMode(800, 600), __FILE__);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            } else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}
