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


Tileset<SnakeGame::Tile> loadTileset() {
    sf::Texture texture;
    texture.loadFromFile("tileset.png");

    using T = SnakeGame::Tile;

    return {
        std::move(texture), 64,
        {
            //{Tile::RED, Tile::BLUE, Tile::GREEN, Tile::YELLOW},
            //{Tile::BLACK}
            {T::APPLE, T::SNAKE, T::HEAD, T::FIELD},
            {T::NIGHT}
        }
    };
}




int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), __FILE__);

    SnakeGame::Environment env(40, 30, 3);
    SnakeGame::State state;

    auto tileset = loadTileset();
    Tilemap<SnakeGame::Tile> tilemap(tileset, 0, 0);
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
                        env.act(SnakeGame::Action::TURN_RIGHT);
                        break;
                    }
                    case sf::Keyboard::Up: {
                        env.act(SnakeGame::Action::TURN_UP);
                        break;
                    }
                    case sf::Keyboard::Left: {
                        env.act(SnakeGame::Action::TURN_LEFT);
                        break;
                    }
                    case sf::Keyboard::Down: {
                        env.act(SnakeGame::Action::TURN_DOWN);
                        break;
                    }
                    case sf::Keyboard::Space: {
                        env.act(SnakeGame::Action::SWAP_HEAD);
                        break;
                    }
                }
            } else if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        env.step();
        state = env.getState();

        tilemap.setMap(state.battlefield);

        window.clear();
        window.draw(tilemap);
        window.display();

        std::cerr << std::boolalpha <<
            "Is winner: " << state.is_winner << '\t' <<
            "Game over: " << state.game_over << std::endl;

        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
