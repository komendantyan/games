#pragma once

#include <SFML/System.hpp>

#include <vector>
#include <map>

namespace TetrisGame {

enum class Tile {
    EMPTY,
    VACANT,
    OCCUPIED,
    CURRENT
};

template <typename Tile>
using Battlefield = std::vector<std::vector<Tile>>;

struct State {  // TODO copy and reference
    Battlefield<Tile> battlefield;
    bool game_over;
    bool is_winner;
};

enum class Action {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    FAST_MOVE_DOWN
};

// Transformations
std::map<sf::Keyboard::Key, Action> keyboardMapping = {
    {sf::Keyboard::Left, Action::MOVE_LEFT},
    {sf::Keyboard::Right, Action::MOVE_RIGHT},
    {sf::Keyboard::Down, Action::MOVE_DOWN},
    {sf::Keyboard::Space, Action::FAST_MOVE_DOWN}
};

}  // namespace TetrisGame
