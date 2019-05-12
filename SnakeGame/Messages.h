#pragma once

#include <SFML/System.hpp>

#include <vector>
#include <map>

namespace SnakeGame {

enum class Tile {
    EMPTY,  // EMPTY,
    APPLE,  // RED,
    SNAKE,  // BLUE,
    HEAD,  // GREEN,
    FIELD,  // YELLOW,
    NIGHT  // BLACK,
};

using Battlefield = std::vector<std::vector<Tile>>;

struct State {  // TODO copy and reference
    Battlefield battlefield;
    bool game_over;
    bool is_winner;
};

enum class Action {
    TURN_RIGHT,
    TURN_UP,
    TURN_LEFT,
    TURN_DOWN,
    SWAP_HEAD
};

// Transformations
std::map<sf::Keyboard::Key, Action> keyboardMapping = {
    {sf::Keyboard::Right, Action::TURN_RIGHT},
    {sf::Keyboard::Up, Action::TURN_UP},
    {sf::Keyboard::Left, Action::TURN_LEFT},
    {sf::Keyboard::Down, Action::TURN_DOWN},
    {sf::Keyboard::Space, Action::SWAP_HEAD}
};

}  // namespace SnakeGame
