#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Tilemap.h"
#include "Environment.h"

#include <iostream>
#include <set>


std::set<sf::Keyboard::Key> ARROW_KEYS = {
    sf::Keyboard::Left,
    sf::Keyboard::Up,
    sf::Keyboard::Right,
    sf::Keyboard::Down
};


Tileset<Tile> loadTileset() {
    sf::Texture texture;
    texture.loadFromFile("tileset.png");
    return {
        std::move(texture), 64,
        {
            //{Tile::RED, Tile::BLUE, Tile::GREEN, Tile::YELLOW},
            //{Tile::BLACK}
            {Tile::APPLE, Tile::SNAKE, Tile::HEAD, Tile::FIELD},
            {Tile::NIGHT}
        }
    };
}




int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), __FILE__);

    Environment env(40, 30);

    auto tileset = loadTileset();
    Tilemap<Tile> tilemap(tileset, 0, 0);
    tilemap.scale(0.25, 0.25);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Escape: {
                        window.close();
                        break;
                    }
                    case sf::Keyboard::Right: {
                        env.act(Environment::Action::TURN_RIGHT);
                        break;
                    }
                    case sf::Keyboard::Up: {
                        env.act(Environment::Action::TURN_UP);
                        break;
                    }
                    case sf::Keyboard::Left: {
                        env.act(Environment::Action::TURN_LEFT);
                        break;
                    }
                    case sf::Keyboard::Down: {
                        env.act(Environment::Action::TURN_DOWN);
                        break;
                    }
                    case sf::Keyboard::Space: {
                        env.act(Environment::Action::SWAP_HEAD);
                        break;
                    }
                }
            } else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        env.step();
        tilemap.setMap(env.getState());

        window.clear();
        window.draw(tilemap);
        window.display();

        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
