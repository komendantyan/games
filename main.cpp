#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Tilemap.h"
#include "Game.h"
#include "SnakeGame/Environment.h"
#include "SnakeGame/Messages.h"
#include "TetrisGame/Environment.h"
#include "TetrisGame/Messages.h"

#include <iostream>


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


Tileset<TetrisGame::Tile> loadTetrisTileset() {
    sf::Texture texture;
    texture.loadFromFile("TetrisGame/tileset.png");

    using T = TetrisGame::Tile;

    return {
        std::move(texture), 64,
        {
            {T::VACANT, T::OCCUPIED, T::CURRENT}
        }
    };
}


int main() {
    sf::RenderWindow window(sf::VideoMode(480, 640), __FILE__);

    auto tileset = loadTetrisTileset();
    Game game(
        window,
        TetrisGame::Environment(30, 40),
        TetrisGame::keyboardMapping,
        tileset
    );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (
                    event.type == sf::Event::Closed ||
                    event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            } else {
                game.processEvent(event);
            }
        }

        game.step();
        game.draw();

        //std::cerr << std::boolalpha <<
        //    "Is winner: " << state.is_winner << '\t' <<
        //    "Game over: " << state.game_over << std::endl;

        sf::sleep(sf::milliseconds(100));
    }

    return 0;
}
